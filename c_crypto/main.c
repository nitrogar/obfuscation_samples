#include "chall.h"
#include "helpers.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

void init_tigress() {}

int main() {
  int sel_chal;
  CLEAR_SCREEN();
  print_banner();
  welcome_message();
  printf("\nPress Enter to continue...\n");
  while (1) {
    present_challenges();
    user_input_int("choose challenge > ", &sel_chal);
    switch (sel_chal) {
    case 1:
      start_challenge1();
      break;
    default:
      printf("[*] please select challenge !!!\n");
    }
  }
  return 0;
}
