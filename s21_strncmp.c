#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;
  int to_exit = 0;

  for (s21_size_t i = 0; i < n && !to_exit; i++) {
    if (str1[i] != str2[i]) {
      result = (int)((unsigned char)str1[i] - (unsigned char)str2[i]);
      to_exit = 1;
    }

    if (!to_exit && str1[i] == '\0') {
      to_exit = 1;
    }

  }

  return result;
}
