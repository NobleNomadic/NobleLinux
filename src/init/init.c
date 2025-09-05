#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mount.h>

int main() {
  // Mount dev
  mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);


  // Drop to shell
  while (1) {
    pid_t pid = fork();

    if (pid == 0) {
      execl("/bin/sh", "sh", NULL);  // make sure you have a shell binary
      _exit(1);
    } else {
      waitpid(pid, NULL, 0);
    }
  }
  return 0;
}

