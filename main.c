#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  while (1) {
    printf("$ ");
    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);
    // Remove the trailing newline
    input[strlen(input) - 1] = '\0';
    if (strcmp(input, "exit") == 0) {
      break;
    } else if (strncmp(input, "echo ", 5) == 0) {
      printf("%s\n", input + 5);
    } else if (strncmp(input, "type ", 5) == 0) {
      char *arg = input + 5;
      if (strcmp(arg, "exit") == 0 || strcmp(arg, "echo") == 0 ||
          strcmp(arg, "type") == 0) {
        printf("%s is a shell builtin\n", arg);
      } else {
        char *path_env = getenv("PATH");
        if (path_env != NULL) {
          char path_copy[1000];
          strncpy(path_copy, path_env, sizeof(path_copy) - 1);
          path_copy[sizeof(path_copy) - 1] = '\0';
          char *dir = strtok(path_copy, ":");
          int found = 0;
          while (dir != NULL && !found) {
            char full_path[1100];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir, arg);
            if (access(full_path, X_OK) == 0) {
              printf("%s is %s\n", arg, full_path);
              found = 1;
            }
            dir = strtok(NULL, ":");
          }
          if (!found) {
            printf("%s: not found\n", arg);
          }
        } else {
          printf("%s: not found\n", arg);
        }
      }
    } else {
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
