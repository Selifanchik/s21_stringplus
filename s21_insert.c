#include <stdlib.h>

#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || start_index > s21_strlen(src)) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = (str == S21_NULL) ? 0 : s21_strlen(str);

  s21_size_t new_len = src_len + str_len;

  char *new_str = (char *)calloc(new_len + 1, sizeof(char));

  if (new_str != S21_NULL) {
    s21_memcpy(new_str, src, start_index);
    if (str != S21_NULL) {
      s21_memcpy(new_str + start_index, str, str_len);
    }
    s21_memcpy(new_str + start_index + str_len, src + start_index,
               src_len - start_index);
    new_str[new_len] = '\0';
  }

  return (void *)new_str;
}
