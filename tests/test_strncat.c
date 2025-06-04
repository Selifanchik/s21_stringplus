#include <check.h>
#include <string.h>

#include "s21_string.h"

// Базовый случай, копируем всю строку
START_TEST(test_strncat_basic) {
  char dest1[20] = "Hello, ";
  char dest2[20] = "Hello, ";
  const char *src = "world!";

  s21_strncat(dest1, src, 6);
  strncat(dest2, src, 6);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Количество копируемых символов меньше строки
START_TEST(test_strncat_n_less_than_src) {
  char dest1[20] = "abc";
  char dest2[20] = "abc";
  const char *src = "defgh";

  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Количество копируемых символов больше строки
START_TEST(test_strncat_n_more_than_src) {
  char dest1[20] = "123";
  char dest2[20] = "123";
  const char *src = "45";

  s21_strncat(dest1, src, 5);
  strncat(dest2, src, 5);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Количество копируемых символов 0
START_TEST(test_strncat_n_zero) {
  char dest1[20] = "cat";
  char dest2[20] = "cat";
  const char *src = "dog";

  s21_strncat(dest1, src, 0);
  strncat(dest2, src, 0);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Копируемая строка пустая
START_TEST(test_strncat_empty_src) {
  char dest1[20] = "start";
  char dest2[20] = "start";
  const char *src = "";

  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Строка назначения пустая
START_TEST(test_strncat_empty_dest) {
  char dest1[20] = "";
  char dest2[20] = "";
  const char *src = "xyz";

  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Все строки пустые
START_TEST(test_strncat_empty_all) {
  char dest1[20] = "";
  char dest2[20] = "";
  const char *src = "";

  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);

  ck_assert_str_eq(dest1, dest2);
}
END_TEST

Suite *strncat_suite(void) {
  Suite *s = suite_create("s21_strncat");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_strncat_basic);
  tcase_add_test(tc, test_strncat_n_less_than_src);
  tcase_add_test(tc, test_strncat_n_more_than_src);
  tcase_add_test(tc, test_strncat_n_zero);
  tcase_add_test(tc, test_strncat_empty_src);
  tcase_add_test(tc, test_strncat_empty_dest);
  tcase_add_test(tc, test_strncat_empty_all);

  suite_add_tcase(s, tc);
  return s;
}
