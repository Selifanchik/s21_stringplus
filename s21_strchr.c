#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *result = S21_NULL;
  int to_exit = 0;

  while (*str != '\0' && !to_exit) {
    if ((unsigned char)*str == (unsigned char)c) {
      result = (char *)str;
      to_exit = 1;
    }
    str++;
  }
  if ((unsigned char)c == '\0') {
    result = (char *)str;
  }

  return result;
}
