#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай
START_TEST(test_to_lower_basic) {
  const char *str1 = "HELLO WORLD";
  const char *str2 = "hello world";

  char *res = (char *)s21_to_lower(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

// Со спец символами
START_TEST(test_to_lower_spec_chair) {
  const char *str1 = "HELLO WORLD!!!\n\t";
  const char *str2 = "hello world!!!\n\t";

  char *res = (char *)s21_to_lower(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

// Со строкой NULL
START_TEST(test_to_lower_null_str) {
  char *res = (char *)s21_to_lower(S21_NULL);
  ck_assert_ptr_null(res);
}
END_TEST

// С пустой строкой
START_TEST(test_to_lower_empty_str) {
  const char *str1 = "";
  const char *str2 = "";

  char *res = (char *)s21_to_lower(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

// Все в нижнем регистре
START_TEST(test_to_lower_all_low) {
  const char *str = "lower";
  char *res = (char *)s21_to_lower(str);

  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, "lower");
  free(res);
}
END_TEST

// Нет букв
START_TEST(test_to_lower_no_letters) {
  const char *str = "1234!@#";
  char *res = (char *)s21_to_lower(str);

  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, "1234!@#");
  free(res);
}
END_TEST

Suite *to_lower_suite(void) {
  Suite *s = suite_create("s21_to_lower");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_to_lower_basic);
  tcase_add_test(tc_core, test_to_lower_spec_chair);
  tcase_add_test(tc_core, test_to_lower_null_str);
  tcase_add_test(tc_core, test_to_lower_empty_str);
  tcase_add_test(tc_core, test_to_lower_all_low);
  tcase_add_test(tc_core, test_to_lower_no_letters);

  suite_add_tcase(s, tc_core);
  return s;
}