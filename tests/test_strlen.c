#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_strlen_basic) {
  const char *str = "Hello";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

// Массив с заданной длиной
START_TEST(test_strlen_long_arr) {
  const char str[10] = "Hello";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

// Пустая строка
START_TEST(test_strlen_empty_str) {
  const char *str = "";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

// С пробелами
START_TEST(test_strlen_spaces) {
  ck_assert_uint_eq(s21_strlen("    "), strlen("    "));
}
END_TEST

// Спец символы
START_TEST(test_strlen_special_chars) {
  ck_assert_uint_eq(s21_strlen("\n\t\r"), strlen("\n\t\r"));
}
END_TEST

// Длинная строка
START_TEST(test_strlen_long) {
  const char *str =
      "  Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
  ck_assert_uint_eq(s21_strlen(str), strlen(str));
}
END_TEST

Suite *strlen_suite(void) {
  Suite *s = suite_create("s21_strlen");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strlen_basic);
  tcase_add_test(tc_core, test_strlen_long_arr);
  tcase_add_test(tc_core, test_strlen_empty_str);
  tcase_add_test(tc_core, test_strlen_spaces);
  tcase_add_test(tc_core, test_strlen_special_chars);
  tcase_add_test(tc_core, test_strlen_long);

  suite_add_tcase(s, tc_core);
  return s;
}
