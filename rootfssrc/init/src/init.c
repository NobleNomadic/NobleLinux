#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

  // Move into first user process
  // Parse the boot configuration file
  FILE *bootConfigurationFile = fopen("/etc/boot", "r");
  if (bootConfigurationFile == 0) {
    perror("fopen /etc/boot");
    exit(1);
  }

  // Read the first line - path to program to execute first
  char firstProgramPath[256];
  // Run fgets on file and check if read succeeded
  if (fgets(firstProgramPath, sizeof(firstProgramPath), bootConfigurationFile) == NULL) {
    perror("Failed to read from /etc/boot");
    exit(1);    
  }
  fclose(bootConfigurationFile);

  // Remove trailing newline if present
  firstProgramPath[strcspn(firstProgramPath, "\n")] = 0;

  // Start child process found in the config file
  pid_t childPid = fork();
  if (childPid == 0) {
    // Setup new process if fork succeeded
    setgid(1000);
    setuid(1000);
    char *argv[] = {firstProgramPath, NULL};
    execv(argv[0], argv);
    perror("execv failed");
    exit(1);
  }
  // If success, then hang this process
  else if (childPid > 0) {
    waitpid(childPid, NULL, 0);
  }
  // If fork fails, then exit
  else {
    perror("fork failed");
    exit(1);
  }

  // Hang
  for (;;) {}
}
