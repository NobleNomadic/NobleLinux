#include <stdio.h>
#include <dirent.h>

// Entry function
int main() {
  // Set path of current dir
  const char *path = ".";

  // Open directory
  DIR *dir = opendir(path);
  // Catch error to open directory
  if (!dir) {
    perror("Opening directory failed");
    return 1;
  }

  // Iterate through each entry of the directory
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  // Close dir and return
  closedir(dir);
  return 0;
}
