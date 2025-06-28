#include <check.h>
#include <stdlib.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_to_upper_basic) {
  const char *str1 = "hello world";
  const char *str2 = "HELLO WORLD";

  char *res = (char *)s21_to_upper(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

// Со спец символами
START_TEST(test_to_upper_spec_chair) {
  const char *str1 = "hello world!!!\n\t";
  const char *str2 = "HELLO WORLD!!!\n\t";

  char *res = (char *)s21_to_upper(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

// Со строкой NULL
START_TEST(test_to_upper_null_str) {
  char *res = (char *)s21_to_upper(S21_NULL);
  ck_assert_ptr_null(res);
}
END_TEST

// С пустой строкой
START_TEST(test_to_upper_empty_str) {
  const char *str1 = "";
  const char *str2 = "";

  char *res = (char *)s21_to_upper(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

// Все в верхнем регистре
START_TEST(test_to_upper_all_caps) {
  const char *str = "UPPER";
  char *res = (char *)s21_to_upper(str);

  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, "UPPER");
  free(res);
}
END_TEST

// Нет букв
START_TEST(test_to_upper_no_letters) {
  const char *str = "1234!@#";
  char *res = (char *)s21_to_upper(str);

  ck_assert_ptr_nonnull(res);
  ck_assert_str_eq(res, "1234!@#");
  free(res);
}
END_TEST

// Разные символы
START_TEST(test_to_upper_mix) {
  const char *str1 = "abcdefgXYZ123!@#{}~?@";
  const char *str2 = "ABCDEFGXYZ123!@#{}~?@";

  char *res = (char *)s21_to_upper(str1);
  ck_assert_ptr_nonnull(res);

  ck_assert_str_eq(res, str2);
  free(res);
}
END_TEST

Suite *to_upper_suite(void) {
  Suite *s = suite_create("s21_to_upper");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_to_upper_basic);
  tcase_add_test(tc_core, test_to_upper_spec_chair);
  tcase_add_test(tc_core, test_to_upper_null_str);
  tcase_add_test(tc_core, test_to_upper_empty_str);
  tcase_add_test(tc_core, test_to_upper_all_caps);
  tcase_add_test(tc_core, test_to_upper_no_letters);
  tcase_add_test(tc_core, test_to_upper_mix);

  suite_add_tcase(s, tc_core);
  return s;
}
