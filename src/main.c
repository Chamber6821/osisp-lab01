#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum Error { UNKNOWN_FLAG = 1, TOO_MANY_PATHS = 2 };

enum Flag { LINK, DIRECTORY, REGULAR_FILE, SORT, FLAG_COUNT };

bool enabledFlags[FLAG_COUNT] = {false};

enum Flag char2flag(char ch) {
  switch (ch) {
  case 'l': return LINK;
  case 'd': return DIRECTORY;
  case 'f': return REGULAR_FILE;
  case 's': return SORT;
  default: printf("Unknown flag: %c\n", ch); exit(UNKNOWN_FLAG);
  }
}

bool hasFlags() {
  for (int i = 0; i < FLAG_COUNT; i++)
    if (enabledFlags[i]) return true;
  return false;
}

bool scanFlags(const char *argument) {
  if (argument[0] != '-') return false;
  for (const char *it = argument + 1; it; ++it)
    enabledFlags[char2flag(*it)] = true;
  return true;
}

int main(int argc, char **argv) {
  char *path = NULL;
  for (int i = 0; i < argc; i++) {
    char *argument = argv[i];
    bool isFlag = scanFlags(argument);
    if (!isFlag && !path) path = argument;
    if (!isFlag && path) {
      printf(
          "Too many paths: path must be once, but got second path: %s",
          argument
      );
      exit(TOO_MANY_PATHS);
    }
  }

  // TODO: Пора бы и вывести в консоль что-нибудь
}
