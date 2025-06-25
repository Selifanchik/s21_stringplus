#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  const char *result = S21_NULL;
  int to_exit = 0;

  if (*needle == '\0') {
    result = haystack;
  } else {
    for (s21_size_t i = 0; haystack[i] != '\0' && !to_exit; i++) {
      s21_size_t len = 0;
      while (needle[len] != '\0' && haystack[i + len] == needle[len]) {
        len++;
      }
      if (needle[len] == '\0') {
        result = &haystack[i];
        to_exit = 1;
      }
    }
  }

  return (char *)result;
}
