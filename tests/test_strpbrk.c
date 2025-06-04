#include <check.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай
START_TEST(test_strpbrk_found) {
  const char *str1 = "hello world";
  const char *str2 = "od";

  char *res1 = s21_strpbrk(str1, str2);
  char *res2 = strpbrk(str1, str2);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Нет совпадений
START_TEST(test_strpbrk_not_found) {
  const char *str1 = "hello world";
  const char *str2 = "cf";

  char *res1 = s21_strpbrk(str1, str2);
  char *res2 = strpbrk(str1, str2);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Совпадение в первом символе
START_TEST(test_strpbrk_first_char_match) {
  const char *str1 = "abc";
  const char *str2 = "a";
  ck_assert_ptr_eq(s21_strpbrk(str1, str2), strpbrk(str1, str2));  // 'a'
}
END_TEST

// Совпадение в последнем сивволе
START_TEST(test_strpbrk_last_char_match) {
  const char str1[10] = "xyz";
  const char str2[10] = "z";
  ck_assert_ptr_eq(s21_strpbrk(str1, str2), strpbrk(str1, str2));  // 'z'
}
END_TEST

// Ищем нулевую строку
START_TEST(test_strpbrk_null_str) {
  const char *str1 = "hello world";
  const char *str2 = "\0";

  char *res1 = s21_strpbrk(str1, str2);
  char *res2 = strpbrk(str1, str2);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем в пустой строке
START_TEST(test_strpbrk_empty_str1) {
  const char *str1 = "";
  const char *str2 = "cf";

  char *res1 = s21_strpbrk(str1, str2);
  char *res2 = strpbrk(str1, str2);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем в пустой строке пустую строку
START_TEST(test_strpbrk_empty_str2) {
  const char *str1 = "";
  const char *str2 = "\0";

  char *res1 = s21_strpbrk(str1, str2);
  char *res2 = strpbrk(str1, str2);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем спец символы
START_TEST(test_strpbrk_str_with_special_chars) {
  const char *str1 = "adf\n\t";
  const char *str2 = "s\n";

  char *res1 = s21_strpbrk(str1, str2);
  char *res2 = strpbrk(str1, str2);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

Suite *strpbrk_suite(void) {
  Suite *s = suite_create("s21_strpbrk");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strpbrk_found);
  tcase_add_test(tc_core, test_strpbrk_not_found);
  tcase_add_test(tc_core, test_strpbrk_first_char_match);
  tcase_add_test(tc_core, test_strpbrk_last_char_match);
  tcase_add_test(tc_core, test_strpbrk_null_str);
  tcase_add_test(tc_core, test_strpbrk_empty_str1);
  tcase_add_test(tc_core, test_strpbrk_empty_str2);
  tcase_add_test(tc_core, test_strpbrk_str_with_special_chars);

  suite_add_tcase(s, tc_core);
  return s;
}