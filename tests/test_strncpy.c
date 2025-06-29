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
  ck_assert_mem_eq(dest1, dest2, 7);
}
END_TEST

// n меньше копируемой строки (для корректности  инициализируем строки
// назначения нулями)
START_TEST(test_strncpy_n_less_src) {
  const char *src = "abcdef";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 3);
  strncpy(dest2, src, 3);
  ck_assert_mem_eq(dest1, dest2, 3);
}
END_TEST

// n равна копируемой строке, но без нуль-терминатора
START_TEST(test_strncpy_no_null_terminator) {
  const char *src = "abc def";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 7);
  strncpy(dest2, src, 7);
  ck_assert_mem_eq(dest1, dest2, 7);
}
END_TEST

// Копируемая строка пустая
START_TEST(test_strncpy_zero_src) {
  const char *src = "";
  char dest1[10] = "helloworl";
  char dest2[10] = "helloworl";
  s21_strncpy(dest1, src, 5);
  strncpy(dest2, src, 5);
  ck_assert_mem_eq(dest1, dest2, 5);
}
END_TEST

// Спец символы
START_TEST(test_strncpy_spec_char) {
  const char *src = "abcde \n\t";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 10);
  strncpy(dest2, src, 10);
  ck_assert_mem_eq(dest1, dest2, 10);
}
END_TEST

// Перекрытые
START_TEST(test_strncpy_overlap) {
  const char *src = "abr";
  char dest1[10] = "abcdef";
  char dest2[10] = "abcdef";
  s21_strncpy(dest1, src, 3);
  strncpy(dest2, src, 3);
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
  tcase_add_test(tc_core, test_strncpy_zero_src);
  tcase_add_test(tc_core, test_strncpy_spec_char);
  tcase_add_test(tc_core, test_strncpy_overlap);

  suite_add_tcase(s, tc_core);
  return s;
}
