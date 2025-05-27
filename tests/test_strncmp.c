#include <check.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай, строки равны
START_TEST(test_strncmp_equal) {
  const char *str1 = "abcdef";
  const char *str2 = "abcdef";
  ck_assert_int_eq(s21_strncmp(str1, str2, 6), strncmp(str1, str2, 6));
}
END_TEST

// Строки не равны, результат отрицательный
START_TEST(test_strncmp_less) {
  const char *str1 = "abcde";
  const char *str2 = "abcz";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5), strncmp(str1, str2, 5));
}
END_TEST

// Строки не равны, результат положительный
START_TEST(test_strncmp_greater) {
  const char *str1 = "abcz";
  const char *str2 = "abcde";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5), strncmp(str1, str2, 5));
}
END_TEST

// Строки равны в пределах n
START_TEST(test_strncmp_partial_match) {
  const char *str1 = "abc123";
  const char *str2 = "abcXYZ";
  ck_assert_int_eq(s21_strncmp(str1, str2, 3), strncmp(str1, str2, 3));
}
END_TEST

// Длина поиска 0
START_TEST(test_strncmp_zero_length) {
  const char *str1 = "abc";
  const char *str2 = "xyz";
  ck_assert_int_eq(s21_strncmp(str1, str2, 0), strncmp(str1, str2, 0));
}
END_TEST

// Строки пустые
START_TEST(test_strncmp_empty_strings) {
  const char *str1 = "";
  const char *str2 = "";
  ck_assert_int_eq(s21_strncmp(str1, str2, 6), strncmp(str1, str2, 6));
}
END_TEST

Suite *strncmp_suite(void) {
  Suite *s = suite_create("s21_strncmp");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strncmp_equal);
  tcase_add_test(tc_core, test_strncmp_less);
  tcase_add_test(tc_core, test_strncmp_greater);
  tcase_add_test(tc_core, test_strncmp_partial_match);
  tcase_add_test(tc_core, test_strncmp_zero_length);
  tcase_add_test(tc_core, test_strncmp_empty_strings);

  suite_add_tcase(s, tc_core);
  return s;
}