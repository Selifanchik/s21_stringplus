#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай: полное копирование
START_TEST(test_memcpy_basic) {
  const char src[] = "hello";
  char dest[6] = {0};
  char expected[6] = {0};

  memcpy(expected, src, 6);
  s21_memcpy(dest, src, 6);

  ck_assert_mem_eq(dest, expected, 6);
}
END_TEST

// Частичное копирование строки
START_TEST(test_memcpy_partial) {
  const char src[] = "abcdef";
  char dest[6] = {0};
  char expected[6] = {0};

  memcpy(expected, src, 3);
  s21_memcpy(dest, src, 3);

  ck_assert_mem_eq(dest, expected, 3);
}
END_TEST

// С нулями
START_TEST(test_memcpy_with_nulls) {
  const char src[] = {'a', '\0', 'c', '\0', 'e'};
  char dest[5] = {0};
  char expected[5] = {0};

  memcpy(expected, src, 5);
  s21_memcpy(dest, src, 5);

  ck_assert_mem_eq(dest, expected, 5);
}
END_TEST

// С байтами, не строками
START_TEST(test_memcpy_unsigned) {
  unsigned const char src[] = {255, 0, 127, 1};
  unsigned char dest[4] = {0};
  unsigned char expected[4] = {0};

  memcpy(expected, src, 4);
  s21_memcpy(dest, src, 4);

  ck_assert_mem_eq(dest, expected, 4);
}
END_TEST

// Длина равна нулю
START_TEST(test_memcpy_zero_length) {
  const char src[] = "abcdef";
  char dest[] = "123456";
  char expected[] = "123456";

  memcpy(expected, src, 0);
  s21_memcpy(dest, src, 0);

  ck_assert_str_eq(dest, expected);
}
END_TEST

// Пустая строка
START_TEST(test_memcpy_empty_str) {
  const char src[] = "";
  char dest[] = "123456";
  char expected[] = "123456";

  memcpy(expected, src, 1);
  s21_memcpy(dest, src, 1);

  ck_assert_mem_eq(dest, expected, 7);
}
END_TEST

// Пустые строки
START_TEST(test_memcpy_empty_strs) {
  const char src[] = "";
  char dest[2];
  char expected[2];

  memcpy(expected, src, 1);
  s21_memcpy(dest, src, 1);

  ck_assert_mem_eq(dest, expected, 1);
}
END_TEST

Suite *memcpy_suite(void) {
  Suite *s = suite_create("s21_memcpy");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_memcpy_basic);
  tcase_add_test(tc, test_memcpy_partial);
  tcase_add_test(tc, test_memcpy_with_nulls);
  tcase_add_test(tc, test_memcpy_unsigned);
  tcase_add_test(tc, test_memcpy_zero_length);
  tcase_add_test(tc, test_memcpy_empty_str);
  tcase_add_test(tc, test_memcpy_empty_strs);

  suite_add_tcase(s, tc);
  return s;
}
