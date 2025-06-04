#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *result = S21_NULL;

  if (*needle == '\0') {
    result = (char *)haystack;
  } else {
    for (s21_size_t i = 0; haystack[i] != '\0'; i++) {
      s21_size_t len = 0;
      while (needle[len] != '\0' && haystack[i + len] == needle[len]) {
        len++;
      }
      if (needle[len] == '\0') {
        result = (char *)&haystack[i];
        break;
      }
    }
  }

  return result;
}