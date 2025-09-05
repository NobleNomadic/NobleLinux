#include <unistd.h>

int main() {
  // Clear screen and reset cursor
  write(1, "\x1B[2J", sizeof("\x1B[2J"));
  write(1, "\033[H", sizeof("\033[H"));

  // Print prompt
  write(1, "# ", 2);
  while (1) {}
}
