#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int to_exit = 0;

  for (s21_size_t i = 0; str1[i] != '\0' && !to_exit; i++) {
    for (s21_size_t j = 0; str2[j] != '\0' && !to_exit; j++) {
      if (str1[i] == str2[j]) {
        to_exit = 1;
      }
    }
    if (!to_exit) {
      len++;
    } 
  }
  return len;
}
