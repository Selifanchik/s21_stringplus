#include <check.h>
#include <string.h>

#include "../s21_strerror.h"
#include "../s21_string.h"

START_TEST(test_strerror_valid) {
  for (int i = 0; i < ERR_N; i++) {
    char *result = s21_strerror(i);
    char *original = strerror(i);
    ck_assert_str_eq(result, original);
  }
}
END_TEST

START_TEST(test_strerror_negativ) {
  for (int i = -100; i < 0; i++) {
    char *result = s21_strerror(i);
    char *original = strerror(i);
    ck_assert_str_eq(result, original);
  }
}
END_TEST

START_TEST(test_strerror_more_then_errors) {
  for (int i = ERR_N; i < 150; i++) {
    char *result = s21_strerror(i);
    char *original = strerror(i);
    ck_assert_str_eq(result, original);
  }
}
END_TEST

Suite *strerror_suite(void) {
  Suite *s = suite_create("s21_strerror");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strerror_valid);
  tcase_add_test(tc_core, test_strerror_negativ);
  tcase_add_test(tc_core, test_strerror_more_then_errors);

  suite_add_tcase(s, tc_core);
  return s;
}
