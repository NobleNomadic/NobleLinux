#include <unistd.h>

// Entry function
int main() {
  // Print escape sequence to clear screen
  write(1, "\033[2J\033[H", 7);
  return 0;
}
