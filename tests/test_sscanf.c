#include <check.h>
#include <stdio.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_sscanf_basic) {
  const char *str = "   1Hello, world!   ";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
}
END_TEST

Suite *sscanf_suite(void) {
  Suite *s = suite_create("s21_sscanf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sscanf_basic);

  suite_add_tcase(s, tc_core);
  return s;
}