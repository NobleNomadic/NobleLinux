#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Spawn the new process
void processCommand(char *commandBuffer) {
  pid_t childPid = fork();
  if (childPid == 0) {
    // Child process: execute the command
    char *args[256];
    int i = 0;

    // Extract arguments
    args[i] = strtok(commandBuffer, " ");
    while (args[i] != NULL && i < 255) {
      i++;
      args[i] = strtok(NULL, " ");
    }

    execvp(args[0], args);

    // execvp only returns if it fails
    perror("execvp failed");
    _exit(1);
  } else if (childPid > 0) {
    // Parent process: wait for child to finish
    wait(NULL);
  } else {
    perror("fork failed");
  }
}

// Entry function
int main() {
  // Definition of prompt
  char shellPrompt[] = "# ";

  while (1) {
    // Print prompt
    printf("%s", shellPrompt);
    fflush(stdout);

    // Get input from user
    char commandBuffer[256];
    if (fgets(commandBuffer, sizeof(commandBuffer), stdin) == NULL) {
      // EOF or error, exit shell
      break;
    }

    // Remove trailing newline
    commandBuffer[strcspn(commandBuffer, "\n")] = 0;

    // Skip empty input
    if (strlen(commandBuffer) == 0) {
      continue;
    }

    processCommand(commandBuffer);
  }

  return 0;
}
