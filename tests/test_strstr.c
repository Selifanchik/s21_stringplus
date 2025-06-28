#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_strstr_found) {
  const char *haystack = "hello world";
  const char *needle = "world";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Строка не найдена
START_TEST(test_strstr_not_found) {
  const char *haystack = "hello world";
  const char *needle = "dy";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Сроки одинаковые
START_TEST(test_strstr_identic) {
  const char *haystack = "hello world";
  const char *needle = "hello world";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// Нулевой символ
START_TEST(test_strstr_null_char) {
  const char *haystack = "hello world";
  const char *needle = "\0";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);//на начало haystack 
}
END_TEST

// Ищем в пустой строке
START_TEST(test_strstr_in_empty_str) {
  const char *haystack = "";
  const char *needle = "sdfsf";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);//NULL
}
END_TEST

// Ищем пустую строку
START_TEST(test_strstr_found_empty_str) {
  const char *haystack = "hello";
  const char *needle = "";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);//на начало haystack 
}
END_TEST

// Ищем в пустой строке нулевой символ
START_TEST(test_strstr_empty_all) {
  const char *haystack = "";
  const char *needle = "";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);//на начало haystack 
}
END_TEST

// Ищем спецсимвол
START_TEST(test_strstr_special_char) {
  const char *haystack = "abc\na";
  const char *needle = "\na";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

// С перекрытиями
START_TEST(test_strstr_overlap) {
  const char *haystack = "ababaaba";
  const char *needle = "bab";

  char *res1 = s21_strstr(haystack, needle);
  char *res2 = strstr(haystack, needle);

  ck_assert_ptr_eq(res1, res2);
}
END_TEST

Suite *strstr_suite(void) {
  Suite *s = suite_create("s21_strstr");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strstr_found);
  tcase_add_test(tc_core, test_strstr_not_found);
  tcase_add_test(tc_core, test_strstr_identic);
  tcase_add_test(tc_core, test_strstr_null_char);
  tcase_add_test(tc_core, test_strstr_in_empty_str);
  tcase_add_test(tc_core, test_strstr_found_empty_str);
  tcase_add_test(tc_core, test_strstr_empty_all);
  tcase_add_test(tc_core, test_strstr_special_char);
  tcase_add_test(tc_core, test_strstr_overlap);

  suite_add_tcase(s, tc_core);
  return s;
}
