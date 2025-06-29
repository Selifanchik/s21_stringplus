#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  const char *ptr = S21_NULL;
  int to_exit = 0;

  for (s21_size_t i = 0; str1[i] != '\0' && !to_exit; i++) {
    for (s21_size_t j = 0; str2[j] != '\0' && !to_exit; j++) {
      if (str1[i] == str2[j]) {
        ptr = &str1[i];
        to_exit = 1;
      }
    }
  }

  return (char *)ptr;
}
