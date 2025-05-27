#include <check.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай
START_TEST(test_strcspn_basic) {
  const char *str1 = "Hello";
  const char *str2 = "World";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// Совпадение первого символа в конце второй строки
START_TEST(test_strcspn_basic2) {
  const char *str1 = "abcde";
  const char *str2 = "edcba";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// Нет совпадений
START_TEST(test_strcspn_no_matches) {
  const char *str1 = "Hello";
  const char *str2 = "abcd";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// Пустая строка для поиска
START_TEST(test_strcspn_empty_str1) {
  const char *str1 = "";
  const char *str2 = "Hello";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// Пустая строка для поиска
START_TEST(test_strcspn_empty_str2) {
  const char *str1 = "Hello";
  const char *str2 = "";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// Строки одинаковые
START_TEST(test_strcspn_equal) {
  const char *str1 = "Hello";
  const char *str2 = "Hello";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// С регистром
START_TEST(test_strcspn_reg) {
  const char *str1 = "HELLO";
  const char *str2 = "hello";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

// С регистром
START_TEST(test_strcspn_empty_all) {
  const char *str1 = "";
  const char *str2 = "";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

Suite *strcspn_suite(void) {
  Suite *s = suite_create("s21_strcspn");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strcspn_basic);
  tcase_add_test(tc_core, test_strcspn_basic2);
  tcase_add_test(tc_core, test_strcspn_no_matches);
  tcase_add_test(tc_core, test_strcspn_empty_str1);
  tcase_add_test(tc_core, test_strcspn_empty_str2);
  tcase_add_test(tc_core, test_strcspn_equal);
  tcase_add_test(tc_core, test_strcspn_reg);
  tcase_add_test(tc_core, test_strcspn_empty_all);

  suite_add_tcase(s, tc_core);
  return s;
}