#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *result = S21_NULL;

  int to_exit = 0;

  const unsigned char *ptr = (const unsigned char *)str;
  for (s21_size_t i = 0; i < n && !to_exit; i++) {
    if (ptr[i] == (unsigned char)c) {
      result = (void *)(ptr + i);
      to_exit = 1;
    }
  }
  return result;
}
