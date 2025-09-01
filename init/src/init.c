#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
  write(1, "[*] Noble Linux booted\n", 23);

  // Mount filesystems
  mount("proc", "/proc", "proc", 0, "");
  mount("sysfs", "/sys", "sysfs", 0, "");
  mount("devtmpfs", "/dev", "devtmpfs", 0, "");

  // /tmp directory
  mkdir("/tmp", 01777);
  mount("tmpfs", "/tmp", "tmpfs", 0, "");

  write(1, "[*] Filesystems mounted\n", 24);

  // Try to open /home/.welcome.txt and print contents
  int fd = open("/home/.welcome", O_RDONLY);
  if (fd >= 0) {
    char buf[256];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
      write(1, buf, n);
    }
    close(fd);
  }

    for (;;) {}
}

