#include <check.h>
#include <string.h>
#include <stdio.h>

#include "s21_string.h"

// Базовый случай
START_TEST(test_sscanf_basic) {
  const char *str = "   Hello, world!   ";
  char new_str[20];
  const char *expected = "Hello,";
  sscanf(str,"%s",new_str);
  ck_assert_str_eq(new_str, expected);
}
END_TEST

Suite *sscanf_suite(void) {
  Suite *s = suite_create("s21_sscanf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sscanf_basic);

  suite_add_tcase(s, tc_core);
  return s;
}