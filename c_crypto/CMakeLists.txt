# CMakeLists.txt
cmake_minimum_required(VERSION 3.30)

# Project name and version
project(ObfuscatorChallenge VERSION 1.0)

# Configure C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)
option(ENABLE_MSAN "Enable Memory Sanitizer" OFF)


option(ENABLE_PLAIN_CODE "JUST NORMAL CODE" ON)
option(ENABLE_TIG_OPQ "TIGRESS Opaque Predicates, Branch Functions, and Encoded Arithmetic " ON)
option(ENABLE_TIG_VIRT_MOD "Virtualization and Self-Modification " OFF)
option(ENABLE_TIG_VIRT_OBF "Virtualization and Dynamic Obfuscation  " OFF)
option(ENABLE_TIG_MERG_VIRT_ENCODE "Merge, Virtualization, and Encode Literals  " OFF)
option(ENABLE_TIG_JIT "TIGRESS JIT" OFF)
option(ENABLE_TIG_JIT_DYN "TIGRESS JIT Dynamic" OFF)
option(ENABLE_TIG_VIRT_OBF "Virtualization and Dynamic Obfuscation  " OFF)

if(ENABLE_TIG_OPQ)
  execute_process(tigress --Seed=42 --Statistics=0 --Verbosity=0 --Environment=x86_64:Linux:Gcc:5.1 
        --Transform=InitEntropy \
        --Functions=init_tigress \
        --InitEntropyKinds=vars \
        --Transform=InitOpaque \
        --Functions=init_tigress \
        --InitOpaqueStructs=list,array,env  \
        --Transform=InitBranchFuns \
        --InitBranchFunsCount=1 \
        --Transform=AddOpaque \
        --Functions=main,start_challenge1,hash_round,padding,sbox,xor,add,rotate  \
        --AddOpaqueStructs=list \
        --AddOpaqueKinds=true \
        --Transform=AntiBranchAnalysis \
        --Functions=main,start_challenge1,hash_round,padding,sbox,xor,add,rotate  \
        --AntiBranchAnalysisKinds=branchFuns \
        --AntiBranchAnalysisObfuscateBranchFunCall=false \
        --AntiBranchAnalysisBranchFunFlatten=true \
        --Transform=EncodeArithmetic \
        --Functions=main,start_challenge1,hash_round,padding,sbox,xor,add,rotate  \
        main.c chall_1.c --out=tigress_out.c)

      set(SOURCES tigress_opaque.c  helpers.c)
endif()
if(ENABLE_PLAIN_CODE)
  set(SOURCES main.c chall_1.c helpers.c)
endif()
# Sanitizer configuration function
function(add_sanitizer_flags)
    if(ENABLE_ASAN)
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
        message(STATUS "Address Sanitizer enabled")
    endif()

    if(ENABLE_UBSAN)
        add_compile_options(-fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
        message(STATUS "Undefined Behavior Sanitizer enabled")
    endif()

    if(ENABLE_TSAN)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
        message(STATUS "Thread Sanitizer enabled")
    endif()

    if(ENABLE_MSAN)
        add_compile_options(-fsanitize=memory -fno-omit-frame-pointer)
        add_link_options(-fsanitize=memory)
        message(STATUS "Memory Sanitizer enabled")
    endif()
endfunction()

# Call the sanitizer configuration
# add_sanitizer_flags()


# Add executable
if
add_executable(obfuscator_challenge  SOURCES )

# Set compiler warnings
target_compile_options(obfuscator_challenge  PRIVATE -Wall -Wextra -Wpedantic -g)


# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add custom targets for convenience
add_custom_target(run
    COMMAND obfuscator_challenge
    DEPENDS obfuscator_challenge
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
