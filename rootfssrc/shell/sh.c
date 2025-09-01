// sh.c - Basic shell
#include <unistd.h>

int main() {
  write(1, "User process started\n", 21);
  while (1) {}
}
