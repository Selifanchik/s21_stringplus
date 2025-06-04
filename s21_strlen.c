#include "s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;

  for (s21_size_t i = 0; str[i] != '\0'; i++) {
    len++;
  }

  return len;
}
