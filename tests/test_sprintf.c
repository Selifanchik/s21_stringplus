#include <check.h>
#include <string.h>
#include <stdio.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_sprintf_basic) {
  char str1[20];
  char str2[20];
 
 char a[] = "Hello!";

  sprintf(str1,"%s",a);
  s21_sprintf(str2,"%s",a);
  ck_assert_str_eq(str1,str2);
}
END_TEST

Suite *sprintf_suite(void) {
  Suite *s = suite_create("s21_sprintf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sprintf_basic);

  suite_add_tcase(s, tc_core);
  return s;
}