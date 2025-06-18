#include <stdlib.h>

#include "s21_string.h"

const char *s21_ltrim(const char *src, const char *trim_chars) {
  while (*src && s21_strchr(trim_chars, *src)) {
    src++;
  }
  return src;
}

const char *s21_rtrim(const char *src, const char *trim_chars) {
  const char *end = src + s21_strlen(src) - 1;
  while (s21_strchr(trim_chars, *end)) {
    end--;
  }
  return end;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) return S21_NULL;

  const char *start = s21_ltrim(src, trim_chars);
  char *result;
  if (*start == '\0') {
    result = (char*)malloc(1 * sizeof(char));
    if (result) {
      result[0] = '\0';
    }
  } else {
    const char *end = s21_rtrim(start, trim_chars);
    size_t len = end - start + 1;
    result = (char*)malloc(sizeof(char)*(len + 1));
    if (result) {
      s21_strncpy(result, start, len);
      result[len] = '\0';
    }
  }
  return (void *)result;
}
