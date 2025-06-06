#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *result = S21_NULL;

  while (*str != '\0') {
    if ((unsigned char)*str == (unsigned char)c) {
      result = (char *)str;
      break;
    }
    str++;
  }
  if ((unsigned char)c == '\0') {
    result = (char *)str;
  }
  
  return result;
}