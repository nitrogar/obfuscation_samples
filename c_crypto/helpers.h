#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>


#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...)                                                  \
  fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

void panic(char *msg);
void bytes_tohex(uint8_t *out, uint8_t *in, size_t len);
void clear_stdin();
void present_challenges();
void user_input_int(char *msg, int *in);
void user_input_string(char *msg, uint8_t *buffer);
int hex_tobytes(uint8_t *out, uint8_t *in);
int user_input_bytes(char *msg, uint8_t *buffer);
void user_input_char(char *msg, uint8_t buffer);

void print_bytes(uint8_t *bytes, size_t len) ;
#define CLEAR_SCREEN() printf("\033[H\033[J")

void print_banner();
void welcome_message();
