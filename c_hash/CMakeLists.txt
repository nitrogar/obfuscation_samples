cmake_minimum_required(VERSION 3.30)
project(ObfuscatorChallenge VERSION 1.0)

# Basic configuration
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Sanitizer options
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)
option(ENABLE_MSAN "Enable Memory Sanitizer" OFF)

# Individual Tigress transformations
option(ENABLE_PLAIN_CODE "Just normal code" ON)
option(ENABLE_INIT_ENTROPY "Enable InitEntropy transformation" OFF)
option(ENABLE_INIT_OPAQUE "Enable InitOpaque transformation" ON)
option(ENABLE_INIT_BRANCH_FUNS "Enable InitBranchFuns transformation" OFF)
option(ENABLE_ADD_OPAQUE "Enable AddOpaque transformation" ON)
option(ENABLE_ANTI_BRANCH "Enable AntiBranchAnalysis transformation" OFF)
option(ENABLE_ENCODE_ARITHMETIC "Enable EncodeArithmetic transformation" ON)
option(ENABLE_FLATTEN "Enable Control Flow Flattening" ON)
option(ENABLE_VIRTUALIZE "Enable Function Virtualization" ON)
option(ENABLE_JIT "Enable JIT compilation" OFF)
option(ENABLE_MERGE "Enable Function Merging" OFF)
option(ENABLE_ENCODE_LITERALS "Enable Literals Encoding" OFF)
# Available functions for targeting
set(ALL_FUNCTIONS "main;start_challenge1;hash_round;padding;sbox;xor;add;rotate")

# Function targeting variables for each transformation
set(TIGRESS_SOURCE_OUT "tigress_out.c")
set(INIT_ENTROPY_FUNCTIONS "init_tigress" CACHE STRING "Functions for InitEntropy")
set(INIT_OPAQUE_FUNCTIONS "init_tigress" CACHE STRING "Functions for InitOpaque")
set(INIT_BRANCH_FUNCTIONS "init_tigress" CACHE STRING "Functions for InitBranchFuns")
set(ADD_OPAQUE_FUNCTIONS "main,hash_round" CACHE STRING "Functions for AddOpaque")
set(ANTI_BRANCH_FUNCTIONS "main" CACHE STRING "Functions for AntiBranchAnalysis")
set(ENCODE_ARITHMETIC_FUNCTIONS "lfsr113,sbox" CACHE STRING "Functions for EncodeArithmetic")
set(FLATTEN_FUNCTIONS "rotate" CACHE STRING "Functions for Flatten")
set(VIRTUALIZE_FUNCTIONS "main" CACHE STRING "Functions for Virtualize")
set(JIT_FUNCTIONS "main" CACHE STRING "Functions for JIT")
set(ENCODE_LITERALS_FUNCTIONS "main,start_challenge1" CACHE STRING "Functions for Encode Literals")
set(MERGE_FUNCTIONS "main" CACHE STRING "Functions for Merge")

# Helper function to convert function list to comma-separated string
function(convert_function_list input_list output_var)
    string(REPLACE ";" "," result "${input_list}")
    set(${output_var} "${result}" PARENT_SCOPE)
endfunction()

# Source files configuration
set(COMMON_SOURCES helpers.c)
set(MAIN_SOURCES main.c chall_1.c)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Function to build Tigress command based on enabled transformations
function(build_tigress_command cmd_var)
    set(cmd
        --Seed=42
        --Statistics=0
        --Verbosity=0
        --Environment=x86_64:Linux:Gcc:5.1
    )

    if(ENABLE_INIT_ENTROPY)
        convert_function_list("${INIT_ENTROPY_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=InitEntropy
            --Functions=${FUNC_LIST}
            --InitEntropyKinds=vars
        )
    endif()

    if(ENABLE_INIT_OPAQUE)
        convert_function_list("${INIT_OPAQUE_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=InitOpaque
            --Functions=${FUNC_LIST}
            --InitOpaqueStructs=list,array,env
        )
    endif()
    if(ENABLE_ADD_OPAQUE)
      convert_function_list("${ADD_OPAQUE_FUNCTIONS}" FUNC_LIST)
      list(APPEND cmd
        --Transform=AddOpaque
        --Functions=${FUNC_LIST}
        --AddOpaqueStructs=list
        --AddOpaqueKinds=true
      )
    endif()


    if(ENABLE_INIT_BRANCH_FUNS)
        convert_function_list("${INIT_BRANCH_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=InitBranchFuns
            --Functions=${FUNC_LIST}
            --InitBranchFunsCount=1
        )
    endif()

    
    if(ENABLE_ANTI_BRANCH)
        convert_function_list("${ANTI_BRANCH_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=AntiBranchAnalysis
            --Functions=${FUNC_LIST}
            --AntiBranchAnalysisKinds=branchFuns
            --AntiBranchAnalysisObfuscateBranchFunCall=false
            --AntiBranchAnalysisBranchFunFlatten=true
        )
    endif()

    if(ENABLE_ENCODE_ARITHMETIC)
        convert_function_list("${ENCODE_ARITHMETIC_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=EncodeArithmetic
            --Functions=${FUNC_LIST}
            --EncodeArithmeticKinds=builtin,plugins
        )
    endif()

    if(ENABLE_FLATTEN)
        convert_function_list("${FLATTEN_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=Flatten
            --Functions=${FUNC_LIST}
            --FlattenDispatch=switch
            --FlattenOpaqueStructs=list,array
        )
    endif()

    if(ENABLE_VIRTUALIZE)
        convert_function_list("${VIRTUALIZE_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=Virtualize
            --Functions=${FUNC_LIST}
            --VirtualizeDispatch=switch
            --VirtualizeOperands=stack
        )
    endif()

    if(ENABLE_JIT)
        convert_function_list("${JIT_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=Jit
            --Functions=${FUNC_LIST}
            --JitEncoding=hard
        )
    endif()

    if(ENABLE_ENCODE_LITERALS)
      convert_function_list("${JIT_FUNCTIONS}" FUNC_LIST)
      list(APPEND cmd
        --Transform=EncodeLiterals
        --Functions=${FUNC_LIST}
        --EncodeLiteralsKinds=integer,string
        --EncodeLiteralsIntegerKinds=opaque
      )
    endif()


    if(ENABLE_MERGE)
        convert_function_list("${MERGE_FUNCTIONS}" FUNC_LIST)
        list(APPEND cmd
            --Transform=Merge
            --Functions=${FUNC_LIST}
            --MergeKinds=rename
        )
    endif()

    set(${cmd_var} ${cmd} PARENT_SCOPE)
endfunction()

# Rest of the implementation (sanitizer_flags, run_tigress_obfuscation, etc.) remains the same as in the previous example
# ... (previous implementation) ...

# Sanitizer configuration function
function(add_sanitizer_flags target)
    if(ENABLE_ASAN)
        target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address)
    endif()
    if(ENABLE_UBSAN)
        target_compile_options(${target} PRIVATE -fsanitize=undefined)
        target_link_options(${target} PRIVATE -fsanitize=undefined)
    endif()
    if(ENABLE_TSAN)
        target_compile_options(${target} PRIVATE -fsanitize=thread)
        target_link_options(${target} PRIVATE -fsanitize=thread)
    endif()
    if(ENABLE_MSAN)
        target_compile_options(${target} PRIVATE -fsanitize=memory -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=memory)
    endif()
endfunction()

# Function to run Tigress obfuscation
function(run_tigress_obfuscation output_file)
    build_tigress_command(TIGRESS_CMD)

    execute_process(
        COMMAND tigress
            ${TIGRESS_CMD}
            ${MAIN_SOURCES}
            --out=${output_file}
        RESULT_VARIABLE TIGRESS_RESULT
        OUTPUT_VARIABLE TIGRESS_OUTPUT
        ERROR_VARIABLE TIGRESS_ERROR
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    if(NOT TIGRESS_RESULT EQUAL 0)
        message("Tigress Output: ${TIGRESS_OUTPUT}")
        message("Tigress Error: ${TIGRESS_ERROR}")
        message(FATAL_ERROR "Tigress obfuscation failed with exit code: ${TIGRESS_RESULT}")
    endif()
endfunction()

# Build targets based on configuration
if(ENABLE_PLAIN_CODE)
    add_executable(obfuscator_challenge ${MAIN_SOURCES} ${COMMON_SOURCES})
    target_compile_options(obfuscator_challenge PRIVATE -Wall -Wextra -Wpedantic -g)
    add_sanitizer_flags(obfuscator_challenge)
endif()

# Check if any Tigress transformation is enabled
if(ENABLE_INIT_ENTROPY OR ENABLE_INIT_OPAQUE OR ENABLE_INIT_BRANCH_FUNS OR
   ENABLE_ADD_OPAQUE OR ENABLE_ANTI_BRANCH OR ENABLE_ENCODE_ARITHMETIC OR
   ENABLE_FLATTEN OR ENABLE_VIRTUALIZE OR ENABLE_JIT OR ENABLE_MERGE)


    # Run obfuscation
    run_tigress_obfuscation(${TIGRESS_SOURCE_OUT})

    # Create obfuscated target
    add_executable(obfuscator_challenge_tigress ${TIGRESS_SOURCE_OUT} ${COMMON_SOURCES})
    target_compile_options(obfuscator_challenge_tigress PRIVATE -Wall -Wextra -Wpedantic -g)

    # Add special flags for JIT if enabled
    if(ENABLE_JIT)
        target_compile_options(obfuscator_challenge_tigress PRIVATE -rdynamic)
        target_link_options(obfuscator_challenge_tigress PRIVATE -ldl)
    endif()

    add_sanitizer_flags(obfuscator_challenge_tigress)
endif()
