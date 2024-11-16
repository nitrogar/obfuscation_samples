#include "helpers.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void xor
    (uint8_t * out, uint8_t *in, size_t n) {
      for (size_t i = 0; i < n; i++)
        out[i] = out[i] ^ in[i];
    }


void hash_round(uint8_t *out, uint8_t *in, size_t n) {
  uint8_t cur;
  uint8_t x;
  uint8_t y;  // next byte
  for(size_t i = 0; i < n; i++){
     x = in[i];
     y = 0;
     cur = x ^ y;
     if (cur opt $val)
       if out[yy] = cur operation;

  }
}

void padding(uint8_t *buf, size_t n, size_t limit) {
  for (size_t i = n; i < limit; i++) {
    printf("\nn=%lu ++ buf[%lu] = buf[%lu] * buf[%lu] padding value %d\n", n, i,
           i - n, i - 1, buf[i - n] * buf[i - 1]);
    buf[i] = buf[i - n] * buf[i - 1] % 0xff;
  }
}
void start_challenge2() {
  size_t hash_size = 16;
  uint8_t *buf = malloc(1024);
  size_t input_size;
  printf("[/] Here is a simple hash-like function based on discesion making, try to reverse the code and simplefiey it "
         "algorithem \n");
  input_size = user_input_bytes(
      "[*] You can test the algorithem with your input .... [max size 1024]",
      buf);
  uint8_t *chunk = malloc(hash_size);
  uint8_t *state = malloc(hash_size);
  memset(chunk, 0, hash_size);
  memset(state, 0, hash_size);

  printf("You Entered %lu bytes: ", input_size);
  print_bytes(buf, input_size);
  padding(buf, input_size, hash_size);
  if(input_size < hash_size)
    input_size = hash_size;
  for (size_t feed = 0; feed < input_size / hash_size; feed++) {
    memcpy(chunk, buf + feed * hash_size, hash_size);
    xor(state, chunk, hash_size);

    for (size_t i = 0; i < hash_size; i++) {
      hash_round(state, state, hash_size);
      printf("[%lu]after:  ", i);
      print_bytes(state, hash_size);
      printf("\n");
    }
  }

  free(state);
  free(chunk);
  free(buf);
}
