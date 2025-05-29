#include <check.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай, строки равны
START_TEST(test_strncmp_equal) {
  const char *str1 = "abcdef";
  const char *str2 = "abcdef";
   int my_cmp = s21_strncmp(str1, str2, 6);
  int original_cmp = strncmp(str1, str2, 6);

if (my_cmp < 0)
  ck_assert_int_lt(original_cmp, 0);
else if (my_cmp > 0)
  ck_assert_int_gt(original_cmp, 0);
else
  ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Строки не равны, результат отрицательный
START_TEST(test_strncmp_less) {
  const char *str1 = "abcde";
  const char *str2 = "abcz";
    int my_cmp = s21_strncmp(str1, str2, 5);
  int original_cmp = strncmp(str1, str2, 5);

if (my_cmp < 0)
  ck_assert_int_lt(original_cmp, 0);
else if (my_cmp > 0)
  ck_assert_int_gt(original_cmp, 0);
else
  ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Строки не равны, результат положительный
START_TEST(test_strncmp_greater) {
  const char *str1 = "abcz";
  const char *str2 = "abcde";
    int my_cmp = s21_strncmp(str1, str2, 5);
  int original_cmp = strncmp(str1, str2, 5);

if (my_cmp < 0)
  ck_assert_int_lt(original_cmp, 0);
else if (my_cmp > 0)
  ck_assert_int_gt(original_cmp, 0);
else
  ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Строки равны в пределах n
START_TEST(test_strncmp_partial_match) {
  const char *str1 = "abc123";
  const char *str2 = "abcXYZ";
    int my_cmp = s21_strncmp(str1, str2, 3);
  int original_cmp = strncmp(str1, str2, 3);

if (my_cmp < 0)
  ck_assert_int_lt(original_cmp, 0);
else if (my_cmp > 0)
  ck_assert_int_gt(original_cmp, 0);
else
  ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Длина поиска 0
START_TEST(test_strncmp_zero_length) {
  const char *str1 = "abc";
  const char *str2 = "xyz";
    int my_cmp = s21_strncmp(str1, str2, 0);
  int original_cmp = strncmp(str1, str2, 0);

if (my_cmp < 0)
  ck_assert_int_lt(original_cmp, 0);
else if (my_cmp > 0)
  ck_assert_int_gt(original_cmp, 0);
else
  ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Строки пустые
START_TEST(test_strncmp_empty_strings) {
  const char *str1 = "";
  const char *str2 = "";
    int my_cmp = s21_strncmp(str1, str2, 6);
  int original_cmp = strncmp(str1, str2, 6);

if (my_cmp < 0)
  ck_assert_int_lt(original_cmp, 0);
else if (my_cmp > 0)
  ck_assert_int_gt(original_cmp, 0);
else
  ck_assert_int_eq(original_cmp, 0);
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