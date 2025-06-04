#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_strchr_found) {
  const char *str = "hello world";
  char c = 'o';

  char *res1 = s21_strchr(str, c);
  char *res2 = strchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Символ не найден
START_TEST(test_strchr_not_found) {
  const char *str = "hello world";
  char c = 'z';

  char *res1 = s21_strchr(str, c);
  char *res2 = strchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем нулевой символ
START_TEST(test_strchr_null_char) {
  const char *str = "hello";
  char c = '\0';

  char *res1 = s21_strchr(str, c);
  char *res2 = strchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем в пустой строке
START_TEST(test_strchr_empty_string) {
  const char *str = "";
  char c = 'a';

  char *res1 = s21_strchr(str, c);
  char *res2 = strchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Ищем в пустой строке нулевой символ
START_TEST(test_strchr_empty_string_null_char) {
  const char *str = "";
  char c = '\0';

  char *res1 = s21_strchr(str, c);
  char *res2 = strchr(str, c);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

Suite *strchr_suite(void) {
  Suite *s = suite_create("s21_strchr");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strchr_found);
  tcase_add_test(tc_core, test_strchr_not_found);
  tcase_add_test(tc_core, test_strchr_null_char);
  tcase_add_test(tc_core, test_strchr_empty_string);
  tcase_add_test(tc_core, test_strchr_empty_string_null_char);

  suite_add_tcase(s, tc_core);
  return s;
}
