#include <check.h>
#include <string.h>
#include <stdio.h>

#include <limits.h>
#include <wchar.h>
#include <locale.h>

#include "../s21_string.h"

#define BUF_SIZE 1024

// Базовый случай
START_TEST(test_int_plus_flag) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "test:%% %+d %+i %.0d % d %+10.2d", 42, 42, 1, 0, 123);
  s21_sprintf(buf2, "test:%% %+d %+i %.0d % d %+10.2d", 42, 42, 1, 0, 123);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_int_space_flag) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "% d % i", 42, 42);
  s21_sprintf(buf2, "% d % i", 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_int_zero_flag) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%05d %05i", 42, 42);
  s21_sprintf(buf2, "%05d %05i", 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_int_minus_flag) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%-5d %-5i", 42, 42);
  s21_sprintf(buf2, "%-5d %-5i", 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_int_precision) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.5d %.5i", 42, 42);
  s21_sprintf(buf2, "%.5d %.5i", 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_int_width_and_precision) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%*.*d %8.5i", 8, 5, 42, 42);
  s21_sprintf(buf2, "%*.*d %8.5i", 8, 5, 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_int_width_all) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%+-8.5hd %+-8.5ld %+-8.5hi %+-8.5li", (short)-42, 1234567890L, (short)-42, 1234567890L);
  s21_sprintf(buf2, "%+-8.5hd %+-8.5ld %+-8.5hi %+-8.5li", (short)-42, 1234567890L, (short)-42, 1234567890L);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_unsigned_octal_hex) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.0u %10.6hu %#10.5lo %-#10x %-#10X %#.5o", 0, (short)123, 1234567890L, 48879, 48879, 1);
  s21_sprintf(buf2, "%.0u %10.6hu %#10.5lo %-#10x %-#10X %#.5o", 0, (short)123, 1234567890L, 48879, 48879, 1);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_unsigned_octal_hex2) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%u %#o %#x %#X", 4294967295U, 255, 255, 255);
  s21_sprintf(buf2, "%u %#o %#x %#X", 4294967295U, 255, 255, 255);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_float_precision) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.2f %9.4f %#f %+f % f %+12.4f %012.6f %Lf %#.0f %.0f %10.2f", 3.14159, 3.0000435, 3.0, 3.14159, 3.14159, 0.004356, -3.14159, (long double)3.1234566123441234546876876876, 12.12, 12.12, 12.1234);
  s21_sprintf(buf2, "%.2f %9.4f %#f %+f % f %+12.4f %012.6f %Lf %#.0f %.0f %10.2f", 3.14159, 3.0000435, 3.0, 3.14159, 3.14159, 0.004356, -3.14159, (long double)3.1234566123441234546876876876, 12.12, 12.12, 12.1234);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_exponential_format) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.3e %.3E %#e %#E %-#10.3E %020.10e %e %e %e %e %.1Le", 12345.6789, 12345.6789, 0.01, 1.0, -0.0000123, -0.0000123, 0.000000000002, 0.000, 999999.99, 0.0009999, 0.099999999999L);
  s21_sprintf(buf2, "%.3e %.3E %#e %#E %-#10.3E %020.10e %e %e %e %e %.1Le", 12345.6789, 12345.6789, 0.01, 1.0, -0.0000123, -0.0000123, 0.000000000002, 0.000, 999999.99, 0.0009999, 0.099999999999L);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_g_format) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%g %.4g %#g %G %g %#10.5g %.2g %#.2g %.1g %.0g %.4g %.10g %g %g %#g", 123.456, 0.000123456, 123.0, 1234567.89, 1234567.89, 3.14, 0.0001, -0.0001, 0.0001, 0.0001, 12.0, 1.02, 123456789123.0, 0.00000000001, 99.9999999999);
  s21_sprintf(buf2, "%g %.4g %#g %G %g %#10.5g %.2g %#.2g %.1g %.0g %.4g %.10g %g %g %#g", 123.456, 0.000123456, 123.0, 1234567.89, 1234567.89, 3.14, 0.0001, -0.0001, 0.0001, 0.0001, 12.0, 1.02, 123456789123.0, 0.00000000001, 99.9999999999);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_string_and_char) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%s %10s %.3s %-10.4s %c %5c %-5c %.10s %1s", "hello", "hi", "abcdef", "testing", 'A', 'f', 'B', "abc", "abc");
  s21_sprintf(buf2, "%s %10s %.3s %-10.4s %c %5c %-5c %.10s %1s", "hello", "hi", "abcdef", "testing", 'A', 'f', 'B', "abc", "abc");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_pointer) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  int x = 0;
  sprintf(buf1, "%-15p %5p", (void*)&x, (void*)&x);
  s21_sprintf(buf2, "%-15p %5p", (void*)&x, (void*)&x);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_n){
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  int x = 0;
  int y = 0;
  sprintf(buf1, "abc%d%n", 10, &x);
  s21_sprintf(buf2, "abc%d%n", 10, &y);
  ck_assert_int_eq(x, y);
}
END_TEST

START_TEST(test_wide_char_and_string) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  wchar_t wc = L'Ж';
  wchar_t *ws = L"Привет";
  wchar_t *ws1 = L"\0";
  setlocale(LC_ALL, "C.UTF-8");
  sprintf(buf1, "%5lc %-10.3ls %ls %.20ls %.1ls", wc, ws, ws, ws, ws1);
  s21_sprintf(buf2, "%5lc %-10.3ls %ls %.20ls %.1ls", wc, ws, ws, ws, ws1);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

Suite *sprintf_suite(void) {
  Suite *s = suite_create("s21_sprintf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_int_plus_flag);
  tcase_add_test(tc_core, test_int_space_flag);
  tcase_add_test(tc_core, test_int_zero_flag);
  tcase_add_test(tc_core, test_int_minus_flag);
  tcase_add_test(tc_core, test_int_precision);
  tcase_add_test(tc_core, test_int_width_and_precision);
  tcase_add_test(tc_core, test_int_width_all);
  tcase_add_test(tc_core, test_unsigned_octal_hex);
  tcase_add_test(tc_core, test_unsigned_octal_hex2);
  tcase_add_test(tc_core, test_float_precision);
  tcase_add_test(tc_core, test_exponential_format);
  tcase_add_test(tc_core, test_g_format);
  tcase_add_test(tc_core, test_string_and_char);
  tcase_add_test(tc_core, test_pointer);
  tcase_add_test(tc_core, test_n);

  tcase_add_test(tc_core, test_wide_char_and_string);

  suite_add_tcase(s, tc_core);
  return s;
}