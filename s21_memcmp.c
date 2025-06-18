#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  int to_exit = 0;

  const unsigned char *ptr1 = (const unsigned char *)str1;
  const unsigned char *ptr2 = (const unsigned char *)str2;

  for (s21_size_t i = 0; i < n && !to_exit; i++) {
    if (ptr1[i] != ptr2[i]) {
      result = (int)(ptr1[i] - ptr2[i]);
      to_exit = 1;
    }
  }

  return result;
}
