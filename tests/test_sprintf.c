#include <check.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "../s21_string.h"

#define BUF_SIZE 10000

START_TEST(test_di1) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "test:%% %+d %+i %.0d % d %+10.2d %900.850d", 42, 42, 1, 0, 123,
          -1);
  s21_sprintf(buf2, "test:%% %+d %+i %.0d % d %+10.2d %900.850d", 42, 42, 1, 0,
              123, -1);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_di2) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "% d % i %900.850i %05d %05i %-5d %-5i %.5d %.5i", 42, 42, -1,
          42, 42, 42, 42, 42, 42);
  s21_sprintf(buf2, "% d % i %900.850i %05d %05i %-5d %-5i %.5d %.5i", 42, 42,
              -1, 42, 42, 42, 42, 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_di3) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "% d % i %900.850i %05d %05i %-5d %-5i %.5d %.5i", 7100, 7100,
          7100, 7100, 7100, 7100, 7100, 7100, 7100);
  s21_sprintf(buf2, "% d % i %900.850i %05d %05i %-5d %-5i %.5d %.5i", 7100,
              7100, 7100, 7100, 7100, 7100, 7100, 7100, 7100);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_di4) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "% d % i %900.850i %05d %05i %-5d %-5i %.5d %.5i", -7100, -7100,
          -7100, -7100, -7100, -7100, -7100, -7100, -7100);
  s21_sprintf(buf2, "% d % i %900.850i %05d %05i %-5d %-5i %.5d %.5i", -7100,
              -7100, -7100, -7100, -7100, -7100, -7100, -7100, -7100);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_di5) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%*.*d %8.5i %+-8.5hd %+-8.5ld %+-8.5hi %+-8.5li", 8, 5, 42, 42,
          (short)-42, 1234567890L, (short)-42, 1234567890L);
  s21_sprintf(buf2, "%*.*d %8.5i %+-8.5hd %+-8.5ld %+-8.5hi %+-8.5li", 8, 5, 42,
              42, (short)-42, 1234567890L, (short)-42, 1234567890L);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_di6) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  int min = -2147483648;
  int max = 2147483647;
  long lmax = 9223372036854775807L;
  long lmin = -9223372036854775807L - 1;
  sprintf(buf1, "%d %d %.0d %.0i %+05d %8.5d %ld %ld %hi %hi", min, max, 0, 0,
          1, 42, lmax, lmin, (short)32767, (short)-32768);
  s21_sprintf(buf2, "%d %d %.0d %.0i %+05d %8.5d %ld %ld %hi %hi", min, max, 0,
              0, 1, 42, lmax, lmin, (short)32767, (short)-32768);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_u_x_X_o1) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.0u %10.6hu %#10.5lo %-#10x %-#10X %#.5o", (unsigned int)0, (unsigned short)123,
          1234567890L, 48879, 48879, 1);
  s21_sprintf(buf2, "%.0u %10.6hu %#10.5lo %-#10x %-#10X %#.5o", 0, (short)123,
              1234567890L, 48879, 48879, 1);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_u_x_X_o2) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%u %#o %#x %#X %#0500lx %#300lx", 4294967295U, 255, 255, 255,
          1844674407370955161, 1844674407370955161);
  s21_sprintf(buf2, "%u %#o %#x %#X %#0500lx %#300lx", 4294967295U, 255, 255,
              255, 1844674407370955161, 1844674407370955161);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_u_x_X_o3) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1,
          "%#.5x %#.0x %#.0X %#.0o %#08x %#08X %-#8x %-#8X %#o %#X %#010x "
          "%#010X %lu %hu %ho",
          0, 0, 0, 0, 48879, 48879, 48879, 48879, 8, 255, 255, 255,
          1844674407370955161UL, (unsigned short)123, (unsigned short)123);
  s21_sprintf(buf2,
              "%#.5x %#.0x %#.0X %#.0o %#08x %#08X %-#8x %-#8X %#o %#X %#010x "
              "%#010X %lu %hu %ho",
              0, 0, 0, 0, 48879, 48879, 48879, 48879, 8, 255, 255, 255,
              1844674407370955161UL, 123, 123);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_u_x_X_o4) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%#x %#.0x %#8.0X %#x %#X %#010x %#12.4X %#-10x %#08x %#x", 0,
          0, 0, 15, 255, 26, 48879, 0xabc, 7, 4294967295U);
  s21_sprintf(buf2, "%#x %#.0x %#8.0X %#x %#X %#010x %#12.4X %#-10x %#08x %#x",
              0, 0, 0, 15, 255, 26, 48879, 0xabc, 7, 4294967295U);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f1) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.2f %9.4f %#f %+f % f %+12.4f %012.6f %Lf %#.0f %.0f %10.2f",
          3.14159, 3.0000435, 3.0, 3.14159, 3.14159, 0.004356, -3.14159,
          (long double)3.1234566123441234546876876876, 12.12, 12.12, 12.1234);
  s21_sprintf(
      buf2, "%.2f %9.4f %#f %+f % f %+12.4f %012.6f %Lf %#.0f %.0f %10.2f",
      3.14159, 3.0000435, 3.0, 3.14159, 3.14159, 0.004356, -3.14159,
      (long double)3.1234566123441234546876876876, 12.12, 12.12, 12.1234);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f2) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%f %f %f %f %f", 0.0, -0.0, 3.14, -3.14, 123456.789);
  s21_sprintf(buf2, "%f %f %f %f %f", 0.0, -0.0, 3.14, -3.14, 123456.789);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f3) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.0f %.1f %.2f %.6f %.10f %.15f %.20f %.25f %.30f %.50f",
          3.14159, 3.14159, 3.14159, 3.14159, 3.14159, 3.14159, 3.14159,
          3.14159, 3.14159, 3.14159);
  s21_sprintf(buf2, "%.0f %.1f %.2f %.6f %.10f %.15f %.20f %.25f %.30f %.50f",
              3.14159, 3.14159, 3.14159, 3.14159, 3.14159, 3.14159, 3.14159,
              3.14159, 3.14159, 3.14159);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f4) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%+f %+f % f % f %#f %#.0f %#.1f %+0f %-f %-#10.2f", 3.14,
          -3.14, 3.14, -3.14, 3.0, 3.0, 3.14, 3.14, 3.14, 3.14);
  s21_sprintf(buf2, "%+f %+f % f % f %#f %#.0f %#.1f %+0f %-f %-#10.2f", 3.14,
              -3.14, 3.14, -3.14, 3.0, 3.0, 3.14, 3.14, 3.14, 3.14);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f5) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1,
          "%10.2f %5.1f %020.10f %-10.2f %0+10.2f %-+10.2f %010.4f % 010.3f "
          "%+020.5f %+#10.2f",
          3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14);
  s21_sprintf(buf2,
              "%10.2f %5.1f %020.10f %-10.2f %0+10.2f %-+10.2f %010.4f % "
              "010.3f %+020.5f %+#10.2f",
              3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14, 3.14);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f6) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%f %f %f %f %f %f %f %f %f %f", 1.0 / 0.0, -1.0 / 0.0,
          NAN, INFINITY, -INFINITY, NAN, -0.0, 0.0, 123.456, -987.654);
  s21_sprintf(buf2, "%f %f %f %f %f %f %f %f %f %f", 1.0 / 0.0, -1.0 / 0.0,
              NAN, INFINITY, -INFINITY, NAN, -0.0, 0.0, 123.456,
              -987.654);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f7) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %0.f", 2.5,
          3.5, 1.5, 4.5, -2.5, -3.5, 2.4999, 2.5001, 9.999, 9.9999, 0.0);
  s21_sprintf(buf2, "%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %0.f",
              2.5, 3.5, 1.5, 4.5, -2.5, -3.5, 2.4999, 2.5001, 9.999, 9.9999,
              0.0);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f8) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  long double val = 3.123456789123456789123456789L;
  sprintf(buf1, "%Lf %20.10Lf %+Lf %#Lf %.0Lf %.20Lf", val, val, val, val, val,
          val);
  s21_sprintf(buf2, "%Lf %20.10Lf %+Lf %#Lf %.0Lf %.20Lf", val, val, val, val,
              val, val);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_f9) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1,
          "%10.2f %5.1f %020.10f %-10.2f %0+10.2f %-+10.2f %010.4f % 010.3f "
          "%+020.5f %+#10.2f",
          12345.6789, 12345.6789, 0.01, 1.0, -0.0000123, -0.0000123,
          0.000000000002, 0.000, 999999.99, 0.0009999);
  s21_sprintf(buf2,
              "%10.2f %5.1f %020.10f %-10.2f %0+10.2f %-+10.2f %010.4f % "
              "010.3f %+020.5f %+#10.2f",
              12345.6789, 12345.6789, 0.01, 1.0, -0.0000123, -0.0000123,
              0.000000000002, 0.000, 999999.99, 0.0009999);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_eE1) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.3e %.3E %#e %#E %-#10.3E %020.10e %e %e %e %e %.1Le",
          12345.6789, 12345.6789, 0.01, 1.0, -0.0000123, -0.0000123,
          0.000000000002, 0.000, 999999.99, 0.0009999, 0.099999999999L);
  s21_sprintf(buf2, "%.3e %.3E %#e %#E %-#10.3E %020.10e %e %e %e %e %.1Le",
              12345.6789, 12345.6789, 0.01, 1.0, -0.0000123, -0.0000123,
              0.000000000002, 0.000, 999999.99, 0.0009999, 0.099999999999L);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_eE2) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%+e %-e % e %0e %#e %+020.5e %e %e %e %e %.0e %e", 3.14, 3.14,
          3.14, 3.14, 3.14, 3.14, 1e-308, 1e-315, 4.9e-315, 1e308, 1e308,
          1.7976931348623157e+308);
  s21_sprintf(buf2, "%+e %-e % e %0e %#e %+020.5e %e %e %e %e %.0e %e", 3.14,
              3.14, 3.14, 3.14, 3.14, 3.14, 1e-308, 1e-315, 4.9e-315, 1e308,
              1e308, 1.7976931348623157e+308);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_eE3) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%e %e %e %e %.6e %.2e %.3e %.0e %.0e", 9.999999e+99,
          1.000000e+100, 9.999999e-100, 1.000000e-99, 9.999999e-01, 9.999e+1,
          9.9999e-1, 9.5, 1.5);
  s21_sprintf(buf2, "%e %e %e %e %.6e %.2e %.3e %.0e %.0e", 9.999999e+99,
              1.000000e+100, 9.999999e-100, 1.000000e-99, 9.999999e-01,
              9.999e+1, 9.9999e-1, 9.5, 1.5);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_eE4) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%+-20.10e %#020.5e %+#020.10e %-#20.10E %.0e %0.e", -123.456,
          0.000123, 123.456, 123.456, 0.123456, 0.123456);
  s21_sprintf(buf2, "%+-20.10e %#020.5e %+#020.10e %-#20.10E %.0e %0.e",
              -123.456, 0.000123, 123.456, 123.456, 0.123456, 0.123456);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_eE5) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%e %e %E %e %E %+e %+E %e %E %e %E %+e %+E", -0.0, 1.0 / 0.0,
          1.0 / 0.0, -1.0 / 0.0, -1.0 / 0.0, 1.0 / 0.0, 1.0 / 0.0, -NAN,
          -NAN, NAN, NAN, NAN, NAN);
  s21_sprintf(buf2, "%e %e %E %e %E %+e %+E %e %E %e %E %+e %+E", -0.0,
              1.0 / 0.0, 1.0 / 0.0, -1.0 / 0.0, -1.0 / 0.0, 1.0 / 0.0,
              1.0 / 0.0, -NAN, -NAN, NAN, NAN, NAN, NAN);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_eE6) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1,
          "%10e %20.10e %.0e %#.0e %.15e %.0e %.0e %.0e %.1e %.1e %.0e %.1e "
          "%-#30.12e %.1e %30.20e %#30.20e",
          3.14, 3.14, 3.14, 3.14, 3.14, 3.5, 1.6, 1.4, 9.94, 9.96, 2.5, 9.95,
          2.2250738585072014e-308, 3.14, 3.14, 3.14);
  s21_sprintf(buf2,
              "%10e %20.10e %.0e %#.0e %.15e %.0e %.0e %.0e %.1e %.1e %.0e "
              "%.1e %-#30.12e %.1e %30.20e %#30.20e",
              3.14, 3.14, 3.14, 3.14, 3.14, 3.5, 1.6, 1.4, 9.94, 9.96, 2.5,
              9.95, 2.2250738585072014e-308, 3.14, 3.14, 3.14);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg1) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1,
          "%g %.4g %#g %G %g %#10.5g %.2g %#.2g %.1g %.0g %.4g %.10g %g %g %#g "
          "%G %g %.0g %.2g",
          123.456, 0.000123456, 123.0, 1234567.89, 1234567.89, 3.14, 0.0001,
          -0.0001, 0.0001, 0.0001, 12.0, 1.02, 123456789123.0, 0.00000000001,
          99.9999999999, 1.0 / 0.0, -99.9999, 123456789123.0, 12345.0);
  s21_sprintf(buf2,
              "%g %.4g %#g %G %g %#10.5g %.2g %#.2g %.1g %.0g %.4g %.10g %g %g "
              "%#g %G %g %.0g %.2g",
              123.456, 0.000123456, 123.0, 1234567.89, 1234567.89, 3.14, 0.0001,
              -0.0001, 0.0001, 0.0001, 12.0, 1.02, 123456789123.0,
              0.00000000001, 99.9999999999, 1.0 / 0.0, -99.9999, 123456789123.0,
              12345.0);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg2) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.1g", 123.0);
  s21_sprintf(buf2, "%.1g", 123.0);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg3) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%g %G %.0g %.1g %.2g %.8g %.15g %8.2g %10.5G", 3.14159,
          3.14159, 0.000123, 123456.0, 0.00000012345, 1234567.89,
          0.000000000000123456, 0.9999999999999, 123456.789);
  s21_sprintf(buf2, "%g %G %.0g %.1g %.2g %.8g %.15g %8.2g %10.5G", 3.14159,
              3.14159, 0.000123, 123456.0, 0.00000012345, 1234567.89,
              0.000000000000123456, 0.9999999999999, 123456.789);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg4) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%+g % g %#g %+#.5g %-10g %010g %+#15.8G", 3.14, 3.14, 3.0,
          123.456, 1.23456, 1.23456, 123456.789);
  s21_sprintf(buf2, "%+g % g %#g %+#.5g %-10g %010g %+#15.8G", 3.14, 3.14, 3.0,
              123.456, 1.23456, 1.23456, 123456.789);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg5) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.5g %.5G %.2g %.2G %.14g %.15G", 1e-300, 1e+300, 9.9999e-10,
          9.9999e+10, 1.234567890123456e-100, 1.234567890123456e+100);
  s21_sprintf(buf2, "%.5g %.5G %.2g %.2G %.14g %.15G", 1e-300, 1e+300,
              9.9999e-10, 9.9999e+10, 1.234567890123456e-100,
              1.234567890123456e+100);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg6) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%g %G %+g %+G %g %G", INFINITY, INFINITY, -INFINITY, -INFINITY,
          NAN, NAN);
  s21_sprintf(buf2, "%g %G %+g %+G %g %G", INFINITY, INFINITY, -INFINITY,
              -INFINITY, NAN, NAN);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg7) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  sprintf(buf1, "%.0g %#.0g %.0G %#.0G %.18g %.18G", 0.9999999999999999, 3.0,
          0.9999999999999999, 3.0, 3.14159265358979323846,
          3.14159265358979323846);
  s21_sprintf(buf2, "%.0g %#.0g %.0G %#.0G %.18g %.18G", 0.9999999999999999,
              3.0, 0.9999999999999999, 3.0, 3.14159265358979323846,
              3.14159265358979323846);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_Gg8) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  long double val = 1.2345678901234567890123456789L;
  sprintf(buf1, "%Lg %.10Lg %20.15Lg", val, val, val);
  s21_sprintf(buf2, "%Lg %.10Lg %20.15Lg", val, val, val);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_s_c) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  char *test_null = NULL;
  sprintf(buf1, "%s %10s %.3s %-10.4s %c %5c %-5c %.10s %1s %-10.8s %10.5s",
          "hello", "hi", "abcdef", "testing", 'A', 'f', 'B', "abc", "abc",
          test_null, test_null);
  s21_sprintf(buf2, "%s %10s %.3s %-10.4s %c %5c %-5c %.10s %1s %-10.8s %10.5s",
              "hello", "hi", "abcdef", "testing", 'A', 'f', 'B', "abc", "abc",
              test_null, test_null);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

/*
cppcheck --enable=all --inconclusive --std=c11 --quiet --force \
  --suppress=missingIncludeSystem \
  --suppress=nullPointer:tests/test_sprintf.c \
  src/ tests/
*/

START_TEST(test_p) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  int x = 0;
  sprintf(buf1, "%-15p %5p", (void *)&x, (void *)&x);
  s21_sprintf(buf2, "%-15p %5p", (void *)&x, (void *)&x);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_n) {
  char buf1[BUF_SIZE], buf2[BUF_SIZE];
  int x = 0;
  int y = 0;
  sprintf(buf1, "abc%900.850d%n", 10, &x);
  s21_sprintf(buf2, "abc%900.850d%n", 10, &y);
  ck_assert_int_eq(x, y);
}
END_TEST

Suite *sprintf_suite(void) {
  Suite *s = suite_create("s21_sprintf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_di1);
  tcase_add_test(tc_core, test_di2);
  tcase_add_test(tc_core, test_di3);
  tcase_add_test(tc_core, test_di4);
  tcase_add_test(tc_core, test_di5);
  tcase_add_test(tc_core, test_di6);
  tcase_add_test(tc_core, test_u_x_X_o1);
  tcase_add_test(tc_core, test_u_x_X_o2);
  tcase_add_test(tc_core, test_u_x_X_o3);
  tcase_add_test(tc_core, test_u_x_X_o4);
  tcase_add_test(tc_core, test_f1);
  tcase_add_test(tc_core, test_f2);
  tcase_add_test(tc_core, test_f3);
  tcase_add_test(tc_core, test_f4);
  tcase_add_test(tc_core, test_f5);
  tcase_add_test(tc_core, test_f6);
  tcase_add_test(tc_core, test_f7);
  tcase_add_test(tc_core, test_f8);
  tcase_add_test(tc_core, test_f9);
  tcase_add_test(tc_core, test_eE1);
  tcase_add_test(tc_core, test_eE2);
  tcase_add_test(tc_core, test_eE3);
  tcase_add_test(tc_core, test_eE4);
  tcase_add_test(tc_core, test_eE5);
  tcase_add_test(tc_core, test_eE6);
  tcase_add_test(tc_core, test_Gg1);
  tcase_add_test(tc_core, test_Gg2);
  tcase_add_test(tc_core, test_Gg3);
  tcase_add_test(tc_core, test_Gg4);
  tcase_add_test(tc_core, test_Gg5);
  tcase_add_test(tc_core, test_Gg6);
  tcase_add_test(tc_core, test_Gg7);
  tcase_add_test(tc_core, test_Gg8);
  tcase_add_test(tc_core, test_s_c);
  tcase_add_test(tc_core, test_p);
  tcase_add_test(tc_core, test_n);

  suite_add_tcase(s, tc_core);
  return s;
}
