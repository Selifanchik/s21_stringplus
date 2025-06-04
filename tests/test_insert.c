#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_insert_basic) {
  const char *src = "Hello I'm from s21";
  const char *str = "world ";

  const char *expected = "Hello world I'm from s21";

  char *res = (char *)s21_insert(src, str, 6);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Вставка в начало
START_TEST(test_insert_at_start) {
  const char *src = "world";
  const char *str = "Hello ";
  const char *expected = "Hello world";

  char *res = (char *)s21_insert(src, str, 0);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Вставка в конец
START_TEST(test_insert_at_end) {
  const char *src = "Hello";
  const char *str = " world";
  const char *expected = "Hello world";

  char *res = (char *)s21_insert(src, str, 5);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Вставка пустой строки
START_TEST(test_insert_empty_str) {
  const char *src = "Hello";
  const char *str = "";
  const char *expected = "Hello";

  char *res = (char *)s21_insert(src, str, 5);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Вставка в пустую строку
START_TEST(test_insert_empty_src) {
  const char *src = "";
  const char *str = "Hello";
  const char *expected = "Hello";

  char *res = (char *)s21_insert(src, str, 0);
  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, expected);
  free(res);
}
END_TEST

// Некорректный индеккс вставки. больше длины строки
START_TEST(test_insert_index_too_large) {
  const char *src = "Hello";
  const char *str = "world";

  char *res = (char *)s21_insert(src, str, 10);
  ck_assert_ptr_null(res);
}
END_TEST

// Строка srt NULL
START_TEST(test_insert_str_null) {
  const char *src = "Hello";
  const char *str = S21_NULL;

  char *res = (char *)s21_insert(src, str, 0);

  ck_assert_ptr_null(res);
}
END_TEST

// Строка src NULL
START_TEST(test_insert_src_null) {
  const char *src = S21_NULL;
  const char *str = "Hello";

  char *res = (char *)s21_insert(src, str, 0);

  ck_assert_ptr_null(res);
}
END_TEST

Suite *insert_suite(void) {
  Suite *s = suite_create("s21_insert");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_insert_basic);
  tcase_add_test(tc_core, test_insert_at_start);
  tcase_add_test(tc_core, test_insert_at_end);
  tcase_add_test(tc_core, test_insert_empty_str);
  tcase_add_test(tc_core, test_insert_empty_src);
  tcase_add_test(tc_core, test_insert_index_too_large);
  tcase_add_test(tc_core, test_insert_str_null);
  tcase_add_test(tc_core, test_insert_src_null);

  suite_add_tcase(s, tc_core);
  return s;
}
