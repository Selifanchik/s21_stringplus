#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;

  while (*ptr != '\0') {
    ptr++;
  }

  for (s21_size_t i = 0; i < n && src[i] != '\0'; i++) {
    *ptr = src[i];
    ptr++;
  }
  *ptr = '\0';

  return dest;
}