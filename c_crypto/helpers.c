#include "helpers.h"
#include <stdint.h>

void panic(char *msg) {
  printf("[!] PANIC!!! : %s ", msg);
  exit(1);
}
void clear_stdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}
void present_challenges() {

  printf("\n(1) Challenge 1\n");
  printf("[*] Challenge 2\n");
  printf("[*] Challenge 3\n");
}

void user_input_int(char *msg, int *in) {
  printf("%s > ", msg);
  scanf("%d", in);
  clear_stdin();
}
void user_input_string(char *msg, uint8_t *buffer) {
  printf("%s > ", msg);
  scanf("%s", buffer);
}
void test_byte_hex(){
  char *in = "0001020304050a0b0c0d0e0faabbccddeeff";
  size_t str_size = strlen(in);
  size_t byte_size = str_size / 2;
  uint8_t *out = malloc(byte_size);
  uint8_t *out2 = malloc(str_size + 1);

  hex_tobytes(out, in);
  for (int i = 0; i < byte_size; i++)
    printf("%02x", out[i]);
  printf("\nDOne printing bytes \n");
  bytes_tohex(out2, out, byte_size);
  printf("%s\n", out2);
}
int hex_tobytes(uint8_t *out, uint8_t *in) {
  size_t len = strlen(in);
  if (len % 2 != 0)
    panic("invalid hex values, hex string must be even");
  for (int i = 0; i < len; i++)
    if (in[i] < '0' || in[i] > 'f')
      panic("invalid hex value, hex string entries must be valid hex value");
  for (int i = 0; i < len / 2; i++) {
    sscanf(in + i * 2, "%2hhx", &out[i]);
  }
  return len / 2;
}

void print_bytes(uint8_t * bytes, size_t len){
  for (int i = 0; i < len; i++)
    printf("%02x", bytes[i]);
};
void bytes_tohex(uint8_t *out, uint8_t *in, size_t len) {
  int pos = 0;
  for (int i = 0; i < len; i++) {
    sprintf(out + i * 2, "%02x", in[i]);
  }
  out[len*2] = '\0';
};
int  user_input_bytes(char *msg, uint8_t *buffer) {
  uint8_t hex_bytes[1024];
  printf("%s > ", msg);
  scanf("%1023s", hex_bytes);

  return hex_tobytes(buffer, hex_bytes);
}
void user_input_char(char *msg, uint8_t buffer) {}

void print_banner() {
  printf("\033[1;36m"); // Cyan color
  printf("   ____  ____  _____ _   _ ____   ____    _  _____ ___  ____  \n");
  printf(
      "  / __ \\| __ )|  ___| | | / ___| / ___|  / \\|_   _/ _ \\|  _ \\ \n");
  printf(
      " | |  | |  _ \\| |_  | | | \\___ \\| |     / _ \\ | || | | | |_) |\n");
  printf(" | |  | | |_) |  _| | |_| |___) | |___ / ___ \\| || |_| |  _ < \n");
  printf("  \\____/|____/|_|    \\___/|____/ \\____/_/   \\_\\_| \\___/|_| "
         "\\_\\\n");
  printf("\n");
  printf("   ____ _   _    _    _     _     _____ _   _  ____ _____ \n");
  printf("  / ___| | | |  / \\  | |   | |   | ____| \\ | |/ ___| ____|\n");
  printf(" | |   | |_| | / _ \\ | |   | |   |  _| |  \\| | |  _|  _|  \n");
  printf(" | |___|  _  |/ ___ \\| |___| |___| |___| |\\  | |_| | |___ \n");
  printf("  \\____|_| |_/_/   \\_\\_____|_____|_____|_| \\_|\\____|_____|\n");
  printf("\033[0m"); // Reset color
}

void welcome_message() {
  printf("\n\033[1;33m"); // Yellow color
  printf("=================================================\n");
  printf("     Welcome to the Code Obfuscation Arena!       \n");
  printf("      Transform your code in unique ways!         \n");
  printf("=================================================\n");
  printf("\033[0m\n"); // Reset color
}
