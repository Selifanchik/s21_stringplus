#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_trim_basic) {
  const char *src = "   Hello, world!   ";
  const char *trim_chars = " ";
  const char *expected = "Hello, world!";
  char *res = (char *)s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Спец символы
START_TEST(test_trim_special_chars) {
  const char *src = "\n\t  Hello, S21!  \t\n";
  const char *trim_chars = " \n\t";
  const char *expected = "Hello, S21!";
  char *res = (char *)s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Вся строка из trim символов
START_TEST(test_trim_all_trimmed) {
  const char *src = "xxx";
  const char *trim_chars = "x";
  const char *expected = "";
  char *res = (char *)s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Нет trim символов
START_TEST(test_trim_no_trim_chars) {
  const char *src = "hello";
  const char *trim_chars = "";
  const char *expected = "hello";
  char *res = (char *)s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Нет trim символов
START_TEST(test_trim_empty_string) {
  const char *src = "";
  const char *trim_chars = " ";
  const char *expected = "";
  char *res = (char *)s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// trim_chars = null
START_TEST(test_trim_null_trim_chars) {
  const char *src = "test";
  char *res = (char *)s21_trim(src, NULL);
  ck_assert_ptr_null(res);
}
END_TEST

// src = null
START_TEST(test_trim_null_src) {
  const char *trim_chars = " ";
  char *res = (char *)s21_trim(NULL, trim_chars);
  ck_assert_ptr_null(res);
}
END_TEST

Suite *trim_suite(void) {
  Suite *s = suite_create("s21_trim");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_trim_basic);
  tcase_add_test(tc_core, test_trim_special_chars);
  tcase_add_test(tc_core, test_trim_all_trimmed);
  tcase_add_test(tc_core, test_trim_no_trim_chars);
  tcase_add_test(tc_core, test_trim_empty_string);
  tcase_add_test(tc_core, test_trim_null_trim_chars);
  tcase_add_test(tc_core, test_trim_null_src);

  suite_add_tcase(s, tc_core);
  return s;
}
