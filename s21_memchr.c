#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *result = S21_NULL;

  const unsigned char *ptr = (const unsigned char *)str;
  for (s21_size_t i = 0; i < n; i++) {
    if (ptr[i] == (unsigned char)c) {
      result = (void *)(ptr + i);
      break;
    }
  }
  return result;
}
