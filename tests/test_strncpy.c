#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай, копируем всю строку
START_TEST(test_strncpy_basic) {
  const char *src = "abc";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 4);
  strncpy(dest2, src, 4);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// n больше копируемой строки (заполнение остатка n нулями)
START_TEST(test_strncpy_n_more_src) {
  const char *src = "abc";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 7);
  strncpy(dest2, src, 7);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// n меньше копируемой строки (для корректности  инициализируем строки
// назначения нулями)
START_TEST(test_strncpy_n_less_src) {
  const char *src = "abcdef";
  char dest1[10] = {'\0'};
  char dest2[10] = {'\0'};
  s21_strncpy(dest1, src, 3);
  strncpy(dest2, src, 3);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// n равна копируемой строке, но без нуль-терминатора
START_TEST(test_strncpy_no_null_terminator) {
  const char *src = "abcdef";
  char dest1[10] = {'\0'};
  char dest2[10] = {'\0'};
  s21_strncpy(dest1, src, 6);
  strncpy(dest2, src, 6);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// //n равна 0
// START_TEST(test_strncpy_n_zero) {
//     const char src[] = "abcdef";
//     char dest1[]="Hello";
//     char dest2[]="Hello";
//     s21_strncpy(dest1, src, 0);
//     strncpy(dest2, src, 0);
//     ck_assert_str_eq(dest1, dest2);
// }
// END_TEST

// Копируемая строка пустая
START_TEST(test_strncpy_zero_src) {
  const char *src = "";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 5);
  strncpy(dest2, src, 5);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

Suite *strncpy_suite(void) {
  Suite *s = suite_create("s21_strncpy");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strncpy_basic);
  tcase_add_test(tc_core, test_strncpy_n_more_src);
  tcase_add_test(tc_core, test_strncpy_n_less_src);
  tcase_add_test(tc_core, test_strncpy_no_null_terminator);
  // tcase_add_test(tc_core, test_strncpy_n_zero);
  tcase_add_test(tc_core, test_strncpy_zero_src);

  suite_add_tcase(s, tc_core);
  return s;
}