#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *ptr1 = (unsigned char *)dest;
  const unsigned char *ptr2 = (const unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) {
    ptr1[i] = ptr2[i];
  }

  return dest;
}
