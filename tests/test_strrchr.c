#include <check.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай
START_TEST(test_strrchr_found) {
  const char *str = "hello world";
  char c = 'o';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Символ не найден
START_TEST(test_strrchr_not_found) {
  const char *str = "hello world";
  char c = 'z';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Символы одинаковые
START_TEST(test_strrchr_identical_char) {
  const char *str = "ffffffffff";
  char c = 'f';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем нулевой символ
START_TEST(test_strrchr_null_char) {
  const char *str = "hello";
  char c = '\0';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем в пустой строке
START_TEST(test_strrchr_empty_string) {
  const char *str = "";
  char c = 'a';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем в пустой строке нулевой символ
START_TEST(test_strrchr_empty_string_null_char) {
  const char *str = "";
  char c = '\0';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем спецсимвол
START_TEST(test_strrchr_special_char) {
  const char str[10] = "ghj\ndss\n";
  char c = '\n';

  char *res1 = s21_strrchr(str, c);
  char *res2 = strrchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

Suite *strrchr_suite(void) {
  Suite *s = suite_create("s21_strrchr");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strrchr_found);
  tcase_add_test(tc_core, test_strrchr_not_found);
  tcase_add_test(tc_core, test_strrchr_identical_char);
  tcase_add_test(tc_core, test_strrchr_null_char);
  tcase_add_test(tc_core, test_strrchr_empty_string);
  tcase_add_test(tc_core, test_strrchr_empty_string_null_char);
  tcase_add_test(tc_core, test_strrchr_special_char);

  suite_add_tcase(s, tc_core);
  return s;
}