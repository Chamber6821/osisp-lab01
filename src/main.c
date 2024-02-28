#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define _DEFAULT_SOURCE
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define error(code, format, ...)                                               \
  do {                                                                         \
    fprintf(stderr, format, __VA_ARGS__);                                      \
    exit(code);                                                                \
  } while (false)

enum Error {
  UNKNOWN_FLAG = 1,
  TOO_MANY_PATHS,
  CANT_OPEN_FILE,
  CANT_GET_STATUS,
  FAILED_SCAN_DIR
};

enum FilterFlag { UNKNOWN_FILTER, LINK, DIRECTORY, REGULAR_FILE, FILTER_COUNT };

enum ModifierFlag { UNKNOWN_MODIFIER, SORT, MODIFIER_COUNT };

bool filters[FILTER_COUNT] = {false};
bool modifiers[MODIFIER_COUNT] = {false};

enum FilterFlag char2filter(char ch) {
  switch (ch) {
  case 'l': return LINK;
  case 'd': return DIRECTORY;
  case 'f': return REGULAR_FILE;
  default: return UNKNOWN_FILTER;
  }
}

enum ModifierFlag char2modifier(char ch) {
  switch (ch) {
  case 's': return SORT;
  default: return UNKNOWN_MODIFIER;
  }
}

bool hasFilters() {
  for (int i = 0; i < FILTER_COUNT; i++)
    if (filters[i]) return true;
  return false;
}

void enableAllFilters() {
  for (int i = 0; i < FILTER_COUNT; i++)
    filters[i] = true;
}

bool scanFlags(const char *argument) {
  if (argument[0] != '-') return false;
  for (const char *it = argument + 1; *it; ++it) {
    enum FilterFlag filter = char2filter(*it);
    if (filter != UNKNOWN_FILTER) {
      filters[filter] = true;
      continue;
    }

    enum ModifierFlag modifier = char2modifier(*it);
    if (modifier != UNKNOWN_MODIFIER) {
      modifiers[modifier] = true;
      continue;
    }

    error(
        UNKNOWN_FLAG,
        "Unknown flag '%c' (%d) in argument '%s'\n",
        *it,
        (int)*it,
        argument
    );
  }
  return true;
}

bool shouldBeShown(mode_t mode) {
  switch (mode & S_IFMT) {
  case S_IFLNK: return filters[LINK];
  case S_IFDIR: return filters[DIRECTORY];
  case S_IFREG: return filters[REGULAR_FILE];
  default: return false;
  }
}

void walk(const char *path) {
  struct stat stat;
  if (lstat(path, &stat))
    error(
        CANT_GET_STATUS,
        "Can't get stat of file %s, error code: %d\n",
        path,
        errno
    );

  if (shouldBeShown(stat.st_mode)) printf("%s\n", path);

  int pathLength = strlen(path);
  if (stat.st_mode & S_IFDIR) {
    DIR *dir = opendir(path);
    if (dir) {
      struct dirent **entries;
      int count = scandir(path, &entries, NULL, alphasort);
      if (count < 0)
        error(
            FAILED_SCAN_DIR,
            "Failed to scan dir '%s', error code: %d\n",
            path,
            errno
        );

      for (int i = 0; i < count; i++) {
        struct dirent entry = *(entries[i]);
        free(entries[i]);
        if (strcmp(entry.d_name, ".") == 0) continue;
        if (strcmp(entry.d_name, "..") == 0) continue;
        char childPath[pathLength + 256];
        walk(strcat(strcat(strcpy(childPath, path), "/"), entry.d_name));
      }
      free(entries);

      closedir(dir);
    }
  }
}

int main(int argc, char **argv) {
  const char *path = NULL;
  for (int i = 1; i < argc; i++) {
    char *argument = argv[i];
    bool isFlag = scanFlags(argument);
    if (!isFlag && path)
      error(
          TOO_MANY_PATHS,
          "Path must be once, but got second path: %s\n",
          argument
      );
    if (!isFlag && !path) path = argument;
  }

  if (!path) path = ".";
  if (!hasFilters()) enableAllFilters();

  walk(path);
}
