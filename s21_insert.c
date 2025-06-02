#include <stdlib.h>

#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL || start_index > s21_strlen(src)) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  s21_size_t new_len = src_len + str_len;

  char *new_str = calloc(new_len + 1, sizeof(char));
  if (new_str == S21_NULL) {
    return S21_NULL;
  }

  s21_strncpy(new_str, src, start_index);
  new_str[start_index] = '\0';
  s21_strncat(new_str, str, str_len);
  s21_strncat(new_str, src + start_index, new_len - start_index - str_len);

  new_str[new_len] = '\0';

  return (void *)new_str;
}