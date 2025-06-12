#include <stdlib.h>

#include "s21_string.h"

void *s21_to_upper(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t len = s21_strlen(str);

  char *new_str = malloc(sizeof(char) * (len + 1));

  if (new_str != S21_NULL) {
    for (s21_size_t i = 0; str[i] != '\0'; i++) {
      if ((unsigned char)str[i] >= 'a' && (unsigned char)str[i] <= 'z') {
        new_str[i] = (unsigned char)str[i] - ('a' - 'A');
      } else {
        new_str[i] = (unsigned char)str[i];
      }
    }
    new_str[len] = '\0';
  }

  return (void *)new_str;
}
