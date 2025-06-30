#include <check.h>
#include <stdio.h>

#include "../s21_string.h"

// * + tests for %c

START_TEST(test_sscanf_char) {
  const char* src = "1He llo, world!   ";
  const char* format = "%c %*c %2c %*2c";
  char std_char1, s21_char1;
  char std_buf1[2], s21_buf1[2];
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_char1, std_buf1);
  s21_count = s21_sscanf(src, format, &s21_char1, s21_buf1);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_mem_eq(std_buf1, s21_buf1, sizeof(char) * 2);
}
END_TEST

START_TEST(test_sscanf_char_sep) {
  const char* src = "ab;cd MHello, 000world!   ";
  const char* format = "%c%*c ;%2c\t %cHello%*2c000%2c";
  char std_char1, s21_char1;
  char std_buf1[2], s21_buf1[2];
  char std_char2, s21_char2;
  char std_buf2[2], s21_buf2[2];
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_char1, std_buf1, &std_char2, std_buf2);
  s21_count =
      s21_sscanf(src, format, &s21_char1, s21_buf1, &s21_char2, s21_buf2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_mem_eq(std_buf1, s21_buf1, sizeof(char) * 2);
  ck_assert_int_eq(std_char2, s21_char2);
  ck_assert_mem_eq(std_buf2, s21_buf2, sizeof(char) * 2);
}
END_TEST

START_TEST(test_sscanf_char_with_spaces) {
  const char* src = "   1 He llo, world!   ";
  const char* format = " %c%c";
  char std_char1, s21_char1;
  char std_char2, s21_char2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_char1, &std_char2);
  s21_count = s21_sscanf(src, format, &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST

START_TEST(test_sscanf_all_ascii_chars) {
  const char* format = "%c";
  for (int i = 0; i < 128; ++i) {
    char src[2] = {0};  // строка из одного символа + '\0'
    src[0] = (char)i;
    char std_char, s21_char;
    int std_count, s21_count;
    std_count = sscanf(src, format, &std_char);
    s21_count = s21_sscanf(src, format, &s21_char);
    ck_assert_msg(std_count == s21_count,
                  "Count mismatch for ASCII %d (0x%02X): std=%d s21=%d", i, i,
                  std_count, s21_count);
    if (std_count == 1 && s21_count == 1) {
      ck_assert_msg(std_char == s21_char,
                    "Char mismatch for ASCII %d (0x%02X): std='%c' s21='%c'", i,
                    i, std_char, s21_char);
    }
  }
}
END_TEST

START_TEST(test_sscanf_char_end_of_string) {
  const char* src = "";
  const char* format = "%c%c";
  char std_char1 = 0, s21_char1 = 0;
  char std_char2 = 0, s21_char2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_char1, &std_char2);
  s21_count = s21_sscanf(src, format, &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST

START_TEST(test_sscanf_char_newline) {
  const char* src = "123\n";
  const char* format = "%*3c%c %c";
  char std_char1, s21_char1;
  char std_char2 = 0, s21_char2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_char1, &std_char2);
  s21_count = s21_sscanf(src, format, &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST

START_TEST(test_sscanf_char_buf) {
  const char* src = "123\n";
  const char* format = "%5c";
  char std_buf[5] = {'\0'}, s21_buf[5] = {'\0'};
  int std_count, s21_count;
  std_count = sscanf(src, format, std_buf);
  s21_count = s21_sscanf(src, format, s21_buf);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_mem_eq(std_buf, s21_buf, sizeof(char) * 5);
}
END_TEST

// * - tests for %d

START_TEST(test_sscanf_int) {
  const char* src = " 1 34 1456-23456 +14 +11 -23478";
  const char* format = "%d %*d %3d %hd %ld %*3d %3hd %4ld";
  int std_int1, std_int2, s21_int1, s21_int2;
  short std_short1, std_short2, s21_short1, s21_short2;
  long std_long1, std_long2, s21_long1, s21_long2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2, &std_short1, &std_long1,
                     &std_short2, &std_long2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, &s21_short1,
                         &s21_long1, &s21_short2, &s21_long2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_short1, s21_short1);
  ck_assert_int_eq(std_short2, s21_short2);
  ck_assert_msg(std_long1 == s21_long1, "Values are not equal: %ld != %ld",
                std_long1, s21_long1);
  ck_assert_msg(std_long2 == s21_long2, "Values are not equal: %ld != %ld",
                std_long2, s21_long2);
}
END_TEST

START_TEST(test_sscanf_int_zeros) {
  const char* src = "0001450 -0001450 +0001450";
  const char* format = "%d\r\v%d%d";
  int std_int1, s21_int1;
  int std_int2, s21_int2;
  int std_int3, s21_int3;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2, &std_int3);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, &s21_int3);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_int3, s21_int3);
}
END_TEST

START_TEST(test_sscanf_int_zero) {
  const char* src = "0 +0; -0f";
  const char* format = "%d\n%d;\t%d";
  int std_int1, s21_int1;
  int std_int2, s21_int2;
  int std_int3, s21_int3;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2, &std_int3);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, &s21_int3);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_int3, s21_int3);
}
END_TEST

START_TEST(test_sscanf_int_incorrect_signs) {
  const char* src = "+-23";
  const char* format = "%d";
  int std_int = 0, s21_int = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int);
  s21_count = s21_sscanf(src, format, &s21_int);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int, s21_int);
}
END_TEST

START_TEST(test_sscanf_int_empty_str) {
  const char* src = "";
  const char* format = "%d";
  int std_int = 0, s21_int = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int);
  s21_count = s21_sscanf(src, format, &s21_int);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int, s21_int);
}
END_TEST

START_TEST(test_sscanf_int_incorrect_chars) {
  const char* src = " Hello, world!   ";
  const char* format = "%d";
  int std_int = 0, s21_int = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int);
  s21_count = s21_sscanf(src, format, &s21_int);
  ck_assert_int_eq(std_int, s21_int);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_int_limits) {
  const char* src1 = "2147483647";
  const char* format = "%d";
  int std_int = 0, s21_int = 0;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int);
  s21_count = s21_sscanf(src1, format, &s21_int);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int, s21_int);
  const char* src2 = "-2147483648";
  std_count = sscanf(src2, format, &std_count);
  s21_count = s21_sscanf(src2, format, &s21_count);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int, s21_int);
}
END_TEST

START_TEST(test_sscanf_d_overflow) {
  const char* src1 =
      "2147483647634784968787820972582959028525 "
      "-65726756275625727562757826528623523756347638196598538";
  const char* format = "%d %d";
  int std_int1, s21_int1, std_int2, s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src1, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
}
END_TEST

// * - tests for %i

START_TEST(test_sscanf_i_dec) {
  const char* src = " 1 34 1456-23456 +14 +11 -23478";
  const char* format = "%i %*i %3i %hi %li %*3i %3hi %4li";
  int std_int1, std_int2, s21_int1, s21_int2;
  short std_short1, std_short2, s21_short1, s21_short2;
  long std_long1, std_long2, s21_long1, s21_long2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2, &std_short1, &std_long1,
                     &std_short2, &std_long2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, &s21_short1,
                         &s21_long1, &s21_short2, &s21_long2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_short1, s21_short1);
  ck_assert_int_eq(std_short2, s21_short2);
  ck_assert_msg(std_long1 == s21_long1, "Values are not equal: %ld != %ld",
                std_long1, s21_long1);
  ck_assert_msg(std_long2 == s21_long2, "Values are not equal: %ld != %ld",
                std_long2, s21_long2);
}
END_TEST

START_TEST(test_sscanf_i_oct) {
  const char* src = " 01 034 014056-023456 +01 +011 -023478";
  const char* format = "%i %*i %3i %hi %li %*3i %4hi %4li";
  int std_int1, std_int2, s21_int1, s21_int2;
  short std_short1, std_short2, s21_short1, s21_short2;
  long std_long1, std_long2, s21_long1, s21_long2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2, &std_short1, &std_long1,
                     &std_short2, &std_long2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, &s21_short1,
                         &s21_long1, &s21_short2, &s21_long2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_short1, s21_short1);
  ck_assert_int_eq(std_short2, s21_short2);
  ck_assert_msg(std_long1 == s21_long1, "Values are not equal: %ld != %ld",
                std_long1, s21_long1);
  ck_assert_msg(std_long2 == s21_long2, "Values are not equal: %ld != %ld",
                std_long2, s21_long2);
}
END_TEST

START_TEST(test_sscanf_i_hex) {
  const char* src = " 0x1 0x3F 0x140X56-0x234B6 +0x1 +0xC1 -0x2A3478";
  const char* format = "%i %*i %4i %hi %li %*4i %3hiC1 %6li";
  int std_int1, std_int2, s21_int1, s21_int2;
  short std_short1, std_short2, s21_short1, s21_short2;
  long std_long1, std_long2, s21_long1, s21_long2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2, &std_short1, &std_long1,
                     &std_short2, &std_long2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, &s21_short1,
                         &s21_long1, &s21_short2, &s21_long2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_short1, s21_short1);
  ck_assert_int_eq(std_short2, s21_short2);
  ck_assert_msg(std_long1 == s21_long1, "Values are not equal: %ld != %ld",
                std_long1, s21_long1);
  ck_assert_msg(std_long2 == s21_long2, "Values are not equal: %ld != %ld",
                std_long2, s21_long2);
}
END_TEST

START_TEST(test_sscanf_i_hex_incorrect) {
  const char* src = "w0x1A3F 0XFF   ";
  const char* format = "%i %i";
  int std_int1 = 0, s21_int1 = 0;
  int std_int2 = 0, s21_int2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_i_oct_incorrect) {
  const char* src = "-01A3F +0XFF   ";
  const char* format = "%i %1i";
  int std_int1 = 0, s21_int1 = 0;
  int std_int2 = 0, s21_int2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_i_octal_incorrect) {
  const char* src = " y01647 0377   ";
  const char* format = "y%i %i";
  int std_int1 = 0, s21_int1 = 0;
  int std_int2 = 0, s21_int2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_i_overflow) {
  const char* src1 =
      "214748364763478496878782 -657267562756257275627578 "
      "0465435425145314536417315 -0465435425145314536417315 "
      "0x78456af7f529ca4562f7b6bd -0x78456af7f529ca4562f7b6bd";
  const char* format = "%i %i %i %i";
  int std_int1, std_int2, std_int3, std_int4;
  int s21_int1, s21_int2, s21_int3, s21_int4;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int1, &std_int2, &std_int3, &std_int4);
  s21_count =
      s21_sscanf(src1, format, &s21_int1, &s21_int2, &s21_int3, &s21_int4);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_int3, s21_int3);
  ck_assert_int_eq(std_int4, s21_int4);
}
END_TEST

// * - tests for %e %E %f %g %G

START_TEST(test_sscanf_e) {
  const char* src = ".3e2 3e2 4e-3 1.234 .0 1.2 1.e-10 8.567";
  const char* format = "%e %*e %5e %le %Le %*3e %3le %3Le";
  float std_float1, std_float2, s21_float1, s21_float2;
  double std_double1, std_double2, s21_double1, s21_double2;
  long double std_ldouble1, std_ldouble2, s21_ldouble1, s21_ldouble2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_float2, &std_double1,
                     &std_ldouble1, &std_double2, &std_ldouble2);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_float2, &s21_double1,
                         &s21_ldouble1, &s21_double2, &s21_ldouble2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_float_eq(std_float1, s21_float1);
  ck_assert_float_eq(std_float2, s21_float2);
  ck_assert_double_eq(std_double1, s21_double1);
  ck_assert_double_eq(std_double2, s21_double2);
  ck_assert_ldouble_eq(std_ldouble1, s21_ldouble1);
  ck_assert_ldouble_eq(std_ldouble2, s21_ldouble2);
}
END_TEST

START_TEST(test_sscanf_E) {
  const char* src = ".3e2 3e2 4e-3 1.234 .0 1.2 1.e-10 8.567";
  const char* format = "%E %*E %5E %lE %LE %*3E %3lE %3LE";
  float std_float1, std_float2, s21_float1, s21_float2;
  double std_double1, std_double2, s21_double1, s21_double2;
  long double std_ldouble1, std_ldouble2, s21_ldouble1, s21_ldouble2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_float2, &std_double1,
                     &std_ldouble1, &std_double2, &std_ldouble2);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_float2, &s21_double1,
                         &s21_ldouble1, &s21_double2, &s21_ldouble2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_float_eq(std_float1, s21_float1);
  ck_assert_float_eq(std_float2, s21_float2);
  ck_assert_double_eq(std_double1, s21_double1);
  ck_assert_double_eq(std_double2, s21_double2);
  ck_assert_ldouble_eq(std_ldouble1, s21_ldouble1);
  ck_assert_ldouble_eq(std_ldouble2, s21_ldouble2);
}
END_TEST

START_TEST(test_sscanf_f) {
  const char* src = ".3e2 3e2 4e-3 1.234 .0 1.2 1.e-10 8.567";
  const char* format = "%f %*f %5f %lf %Lf %*3f %3lf %3Lf";
  float std_float1, std_float2, s21_float1, s21_float2;
  double std_double1, std_double2, s21_double1, s21_double2;
  long double std_ldouble1, std_ldouble2, s21_ldouble1, s21_ldouble2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_float2, &std_double1,
                     &std_ldouble1, &std_double2, &std_ldouble2);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_float2, &s21_double1,
                         &s21_ldouble1, &s21_double2, &s21_ldouble2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_float_eq(std_float1, s21_float1);
  ck_assert_float_eq(std_float2, s21_float2);
  ck_assert_double_eq(std_double1, s21_double1);
  ck_assert_double_eq(std_double2, s21_double2);
  ck_assert_ldouble_eq(std_ldouble1, s21_ldouble1);
  ck_assert_ldouble_eq(std_ldouble2, s21_ldouble2);
}
END_TEST

START_TEST(test_sscanf_g) {
  const char* src = ".3e2 3e2 4e-3 1.234 .0 1.2 1.e-10 8.567";
  const char* format = "%g %*g %5g %lg %Lg %*3g %6lg %3Lg";
  float std_float1, std_float2, s21_float1, s21_float2;
  double std_double1, std_double2, s21_double1, s21_double2;
  long double std_ldouble1, std_ldouble2, s21_ldouble1, s21_ldouble2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_float2, &std_double1,
                     &std_ldouble1, &std_double2, &std_ldouble2);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_float2, &s21_double1,
                         &s21_ldouble1, &s21_double2, &s21_ldouble2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_float_eq(std_float1, s21_float1);
  ck_assert_float_eq(std_float2, s21_float2);
  ck_assert_double_eq(std_double1, s21_double1);
  ck_assert_double_eq(std_double2, s21_double2);
  ck_assert_ldouble_eq(std_ldouble1, s21_ldouble1);
  ck_assert_ldouble_eq(std_ldouble2, s21_ldouble2);
}
END_TEST

START_TEST(test_sscanf_G) {
  const char* src = ".3e2 3e2 4e-3 1.234 .0 1.2 1.e-10 8.567";
  const char* format = "%G %*G %5G %lG %LG %*3G %4lG %3LG";
  float std_float1, std_float2, s21_float1, s21_float2;
  double std_double1, std_double2, s21_double1, s21_double2;
  long double std_ldouble1, std_ldouble2, s21_ldouble1, s21_ldouble2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_float2, &std_double1,
                     &std_ldouble1, &std_double2, &std_ldouble2);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_float2, &s21_double1,
                         &s21_ldouble1, &s21_double2, &s21_ldouble2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_float_eq(std_float1, s21_float1);
  ck_assert_float_eq(std_float2, s21_float2);
  ck_assert_double_eq(std_double1, s21_double1);
  ck_assert_double_eq(std_double2, s21_double2);
  ck_assert_ldouble_eq(std_ldouble1, s21_ldouble1);
  ck_assert_ldouble_eq(std_ldouble2, s21_ldouble2);
}
END_TEST

START_TEST(test_sscanf_e_float_neg_pos_width_delim) {
  const char* src = "\t+.3e2 -3e2: 0.0e0 \n1.234s 0.ee2";
  const char* format = "%e %e: %e %5e %e";
  float std_float1, std_float2, std_float3, std_float4, std_float5 = 1;
  float s21_float1, s21_float2, s21_float3, s21_float4, s21_float5 = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_float2, &std_float3,
                     &std_float4, &std_float5);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_float2, &s21_float3,
                         &s21_float4, &s21_float5);
  ck_assert_float_eq(std_float1, s21_float1);
  ck_assert_float_eq(std_float2, s21_float2);
  ck_assert_float_eq(std_float3, s21_float3);
  ck_assert_float_eq(std_float4, s21_float4);
  ck_assert_float_eq(std_float5, s21_float5);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_e_double) {
  const char* src = ".3e8 3e7 4e-5 455.2345653";
  const char* format = "%le %le %le %le";
  double std_double1, std_double2, std_double3, std_double4;
  double s21_double1, s21_double2, s21_double3, s21_double4;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_double1, &std_double2, &std_double3,
                     &std_double4);
  s21_count = s21_sscanf(src, format, &s21_double1, &s21_double2, &s21_double3,
                         &s21_double4);
  ck_assert_double_eq(std_double1, s21_double1);
  ck_assert_double_eq(std_double2, s21_double2);
  ck_assert_double_eq(std_double3, s21_double3);
  ck_assert_double_eq(std_double4, s21_double4);
  ck_assert_double_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_e_long_double) {
  const char* src = ".3e10 3e11 4e-9 455.234565651";
  const char* format = "%Le %Le %Le %Le";
  long double std_ldouble1, std_ldouble2, std_ldouble3, std_ldouble4;
  long double s21_ldouble1, s21_ldouble2, s21_ldouble3, s21_ldouble4;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ldouble1, &std_ldouble2, &std_ldouble3,
                     &std_ldouble4);
  s21_count = s21_sscanf(src, format, &s21_ldouble1, &s21_ldouble2,
                         &s21_ldouble3, &s21_ldouble4);
  ck_assert_ldouble_eq(std_ldouble1, s21_ldouble1);
  ck_assert_ldouble_eq(std_ldouble2, s21_ldouble2);
  ck_assert_ldouble_eq(std_ldouble3, s21_ldouble3);
  ck_assert_ldouble_eq(std_ldouble4, s21_ldouble4);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_float_exp_incorrect_width) {
  const char* src = "-123.3e2";
  const char* format = "%6E";
  float std_float, s21_float;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float);
  s21_count = s21_sscanf(src, format, &s21_float);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_float_eq(std_float, s21_float);
}
END_TEST

START_TEST(test_sscanf_float_nan_inf) {
  const char* src = "nan inf; .8[1.";
  const char* format = "%Lf %Lf; %Lf [ %Lf";
  long double std_ldouble1, std_ldouble2, std_ldouble3, std_ldouble4;
  long double s21_ldouble1, s21_ldouble2, s21_ldouble3, s21_ldouble4;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ldouble1, &std_ldouble2, &std_ldouble3,
                     &std_ldouble4);
  s21_count = s21_sscanf(src, format, &s21_ldouble1, &s21_ldouble2,
                         &s21_ldouble3, &s21_ldouble4);
  ck_assert_ldouble_nan(std_ldouble1);
  ck_assert_ldouble_nan(s21_ldouble1);
  ck_assert(isinf(std_ldouble2) == isinf(s21_ldouble2));
  ck_assert_ldouble_eq(std_ldouble3, s21_ldouble3);
  ck_assert_ldouble_eq(std_ldouble4, s21_ldouble4);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_correct_inf) {
  const char* src = "-inf inf infi nan";
  const char* format = "%f %lf %3Lf %s %3f";
  float std_float1, s21_float1;
  double std_double, s21_double;
  long double std_ldouble, s21_ldouble;
  float std_float2, s21_float2;
  char std_str[2], s21_str[2];
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_float1, &std_double, &std_ldouble,
                     std_str, &std_float2);
  s21_count = s21_sscanf(src, format, &s21_float1, &s21_double, &s21_ldouble,
                         s21_str, &s21_float2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert(isinf(std_float1) == isinf(s21_float1));
  ck_assert(isinf(std_double) == isinf(s21_double));
  ck_assert(isinf(std_ldouble) == isinf(s21_ldouble));
  ck_assert_str_eq(std_str, s21_str);
  ck_assert_float_nan(std_float2);
  ck_assert_float_nan(s21_float2);
}
END_TEST

START_TEST(test_sscanf_correct_inf2) {
  const char* src = "-iNf infInity InFiNity naN";
  const char* format = "%5Lf %Lf %9lf %Lf";
  long double std_ldouble1, std_ldouble2, std_ldouble3;
  long double s21_ldouble1, s21_ldouble2, s21_ldouble3;
  double std_double, s21_double;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ldouble1, &std_ldouble2, &std_double,
                     &std_ldouble3);
  s21_count = s21_sscanf(src, format, &s21_ldouble1, &s21_ldouble2, &s21_double,
                         &s21_ldouble3);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert(isinf(std_ldouble1) == isinf(s21_ldouble1));
  ck_assert(isinf(std_ldouble2) == isinf(s21_ldouble2));
  ck_assert(isinf(std_double) == isinf(s21_double));
  ck_assert_ldouble_nan(std_ldouble3);
  ck_assert_ldouble_nan(s21_ldouble3);
}
END_TEST

START_TEST(test_sscanf_incorrect_inf) {
  const char* src = "-iNfin inf";
  const char* format = "%lf %s";
  double std_doule = 0, s21_double = 0;
  char std_str[10] = {'\0'}, s21_str[10] = {'\0'};
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_doule, std_str);
  s21_count = s21_sscanf(src, format, &s21_double, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_double_eq(std_doule, s21_double);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

// * - tests for %o

START_TEST(test_sscanf_octal) {
  const char* src = "0567 611 -235 -073 12745 023 -07 677777777777777777777";
  const char* format = "%o %*o %4o %ho %lo %*3o %6ho %5lo";
  unsigned std_uint1, std_uint2;
  unsigned s21_uint1, s21_uint2;
  unsigned short std_ushort1, std_ushort2;
  unsigned short s21_ushort1, s21_ushort2;
  unsigned long std_ulong1, std_ulong2;
  unsigned long s21_ulong1, s21_ulong2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint1, &std_uint2, &std_ushort1,
                     &std_ulong1, &std_ushort2, &std_ulong2);
  s21_count = s21_sscanf(src, format, &s21_uint1, &s21_uint2, &s21_ushort1,
                         &s21_ulong1, &s21_ushort2, &s21_ulong2);
  ck_assert_uint_eq(std_uint1, s21_uint1);
  ck_assert_uint_eq(std_uint2, s21_uint2);
  ck_assert_uint_eq(std_ushort1, s21_ushort1);
  ck_assert_uint_eq(std_ushort2, s21_ushort2);
  ck_assert_msg(std_ulong1 == s21_ulong1, "Values are not equal: %ld != %ld",
                std_ulong1, s21_ulong1);
  ck_assert_msg(std_ulong2 == s21_ulong2, "Values are not equal: %ld != %ld",
                std_ulong2, s21_ulong2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_o_correct_width) {
  const char* src = ";54   0123";
  const char* format = ";%o %1o";
  unsigned std_uint1, std_uint2 = 1;
  unsigned s21_uint1, s21_uint2 = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint1, &std_uint2);
  s21_count = s21_sscanf(src, format, &s21_uint1, &s21_uint2);
  ck_assert_uint_eq(std_uint1, s21_uint1);
  ck_assert_uint_eq(std_uint2, s21_uint2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_o_incorrect_width) {
  const char* src = ";54   90123";
  const char* format = ";%o %2o";
  unsigned std_uint1, std_uint2 = 1;
  unsigned s21_uint1, s21_uint2 = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint1, &std_uint2);
  s21_count = s21_sscanf(src, format, &s21_uint1, &s21_uint2);
  ck_assert_uint_eq(std_uint1, s21_uint1);
  ck_assert_uint_eq(std_uint2, s21_uint2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_o_overflow) {
  const char* src1 = "0465435425145314536417315 -0465435425145314536417315";
  const char* format = "%o %o";
  unsigned std_int1, std_int2;
  unsigned s21_int1, s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src1, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_uint_eq(std_int1, s21_int1);
  ck_assert_uint_eq(std_int2, s21_int2);
}
END_TEST

// * - tests for %s

START_TEST(test_sscanf_str_width) {
  const char* src = "   1Hello, world! ";
  const char* format = "%19s";
  char std_str[20], s21_str[20];
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_str_eq(std_str, s21_str);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_string_with_asterisk) {
  const char* src = "skip keep";
  const char* format = "%*s %s";
  char std_str[100] = {'\0'}, s21_str[100] = {'\0'};
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

START_TEST(test_sscanf_leading_space) {
  const char* src = "     world";
  const char* format = "%s";
  char std_str[10], s21_str[10];
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

START_TEST(test_sscanf_string_with_punctuation) {
  const char* src = "text,more";
  const char* format = "%s";
  char std_str[10], s21_str[10];
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

START_TEST(test_sscanf_only_spaces) {
  const char* src = "   ";
  const char* format = "%s";
  char std_str[10] = {'\0'}, s21_str[10] = {'\0'};
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

START_TEST(test_sscanf_string_width_1) {
  const char* src = "abc";
  const char* format = "%1s";
  char std_str[2] = {'\0'}, s21_str[2] = {'\0'};
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

START_TEST(test_sscanf_width_longer_than_string) {
  const char* src = "short text";
  const char* format = "%9s";
  char std_str[10] = {'\0'}, s21_str[10] = {'\0'};
  int std_count, s21_count;
  std_count = sscanf(src, format, std_str);
  s21_count = s21_sscanf(src, format, s21_str);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

// * - tests for %u

START_TEST(test_sscanf_uint_max) {
  const char* src = "0567 611 -235 -073 12745 023 -07 6777";
  const char* format = "%u %*u %4u %hu %lu %*3u %6hu %5lu";
  unsigned std_uint1, std_uint2;
  unsigned s21_uint1, s21_uint2;
  unsigned short std_ushort1, std_ushort2;
  unsigned short s21_ushort1, s21_ushort2;
  unsigned long std_ulong1, std_ulong2;
  unsigned long s21_ulong1, s21_ulong2;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint1, &std_uint2, &std_ushort1,
                     &std_ulong1, &std_ushort2, &std_ulong2);
  s21_count = s21_sscanf(src, format, &s21_uint1, &s21_uint2, &s21_ushort1,
                         &s21_ulong1, &s21_ushort2, &s21_ulong2);
  ck_assert_uint_eq(std_uint1, s21_uint1);
  ck_assert_uint_eq(std_uint2, s21_uint2);
  ck_assert_uint_eq(std_ushort1, s21_ushort1);
  ck_assert_uint_eq(std_ushort2, s21_ushort2);
  ck_assert_msg(std_ulong1 == s21_ulong1, "Values are not equal: %ld != %ld",
                std_ulong1, s21_ulong1);
  ck_assert_msg(std_ulong2 == s21_ulong2, "Values are not equal: %ld != %ld",
                std_ulong2, s21_ulong2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_u_empty_str) {
  const char* src = "";
  const char* format = "%u";
  unsigned std_uint = 1, s21_uint = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint);
  s21_count = s21_sscanf(src, format, &s21_uint);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_uint_eq(std_uint, s21_uint);
}
END_TEST

START_TEST(test_sscanf_u_overflow) {
  const char* src1 = "214748364763478496878782 -657267562756257275627578";
  const char* format = "%u %u";
  unsigned std_int1, std_int2;
  unsigned s21_int1, s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src1, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_uint_eq(std_int1, s21_int1);
  ck_assert_uint_eq(std_int2, s21_int2);
}
END_TEST
// * - tests for %x %X

START_TEST(test_sscanf_x_correct_width) {
  const char* src = ";54   0x123 0X6AFd 0X654";
  const char* format = ";%x %x %*2x %X %4X %*4X";
  unsigned std_uint1, s21_uint1;
  unsigned std_uint2, s21_uint2;
  unsigned std_uint3, s21_uint3;
  unsigned std_uint4, s21_uint4;
  int std_count, s21_count;
  std_count =
      sscanf(src, format, &std_uint1, &std_uint2, &std_uint3, &std_uint4);
  s21_count =
      s21_sscanf(src, format, &s21_uint1, &s21_uint2, &s21_uint3, &s21_uint4);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_uint_eq(std_uint1, s21_uint1);
  ck_assert_uint_eq(std_uint2, s21_uint2);
  ck_assert_uint_eq(std_uint3, s21_uint3);
  ck_assert_uint_eq(std_uint4, s21_uint4);
}
END_TEST

START_TEST(test_sscanf_x_incorrect_prefix) {
  const char* src = ";0w123";
  const char* format = ";%x";
  unsigned std_uint = 1, s21_uint = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint);
  s21_count = s21_sscanf(src, format, &s21_uint);
  ck_assert_uint_eq(std_uint, s21_uint);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_x_prefix_width) {
  const char* src = ";0X123";
  const char* format = ";%1x";
  unsigned std_uint = 1, s21_uint = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_uint);
  s21_count = s21_sscanf(src, format, &s21_uint);
  ck_assert_uint_eq(std_uint, s21_uint);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_x_overflow) {
  const char* src1 = "0x78456af7f529ca4562f7b6bd -0x78456af7f529ca4562f7b6bd";
  const char* format = "%x %X";
  unsigned std_int1, std_int2;
  unsigned s21_int1, s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src1, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_uint_eq(std_int1, s21_int1);
  ck_assert_uint_eq(std_int2, s21_int2);
}
END_TEST

// * - tests for %p

START_TEST(test_sscanf_p) {
  const char* src = "0xff6732 -fffff -0x124324   111111111111  ";
  const char* format = "%p %*p %*10p %100p";
  void *std_ptr1 = NULL, *std_ptr2 = NULL;
  void *s21_ptr1 = S21_NULL, *s21_ptr2 = S21_NULL;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ptr1, &std_ptr2);
  s21_count = s21_sscanf(src, format, &s21_ptr1, &s21_ptr2);
  ck_assert_ptr_eq(std_ptr1, s21_ptr1);
  ck_assert_ptr_eq(std_ptr2, s21_ptr2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_p_negative) {
  const char* src = "-0xffff k";
  const char* format = "%p %p";
  void *std_ptr1 = NULL, *s21_ptr1 = S21_NULL;
  void *std_ptr2 = NULL, *s21_ptr2 = S21_NULL;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ptr1, &std_ptr2);
  s21_count = s21_sscanf(src, format, &s21_ptr1, &s21_ptr2);
  ck_assert_ptr_eq(std_ptr1, s21_ptr1);
  ck_assert_ptr_eq(std_ptr2, s21_ptr2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_p_neg_pos) {
  const char* src = "-ffff fffffffffffffffffffff -0xfff ";
  const char* format = "%p %p %p";
  void *std_ptr1 = NULL, *s21_ptr1 = S21_NULL;
  void *std_ptr2 = NULL, *s21_ptr2 = S21_NULL;
  void *std_ptr3 = NULL, *s21_ptr3 = S21_NULL;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ptr1, &std_ptr2, &std_ptr3);
  s21_count = s21_sscanf(src, format, &s21_ptr1, &s21_ptr2, &s21_ptr3);
  ck_assert_ptr_eq(std_ptr1, s21_ptr1);
  ck_assert_ptr_eq(std_ptr2, s21_ptr2);
  ck_assert_ptr_eq(std_ptr3, s21_ptr3);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_p_incorrect) {
  const char* src = "qwe123  643";
  const char* format = "%p";
  void *std_ptr = NULL, *s21_ptr = S21_NULL;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_ptr);
  s21_count = s21_sscanf(src, format, &s21_ptr);
  ck_assert_ptr_eq(std_ptr, s21_ptr);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_p_overflow) {
  const char* src1 = "0x78456af7f529ca4562f7b6bd -0x78456af7f529ca4562f7b6bd";
  const char* format = "%p %p";
  void *std_int1, *std_int2;
  void *s21_int1, *s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src1, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src1, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_ptr_eq(std_int1, s21_int1);
  ck_assert_ptr_eq(std_int2, s21_int2);
}
END_TEST

// * - tests for %n

START_TEST(test_sscanf_n) {
  const char* src = " ; 0xff6732";
  const char* format = " ;%n";
  int std_n, s21_n;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_n);
  s21_count = s21_sscanf(src, format, &s21_n);
  ck_assert_int_eq(std_n, s21_n);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_n_count) {
  const char* src = "12345";
  const char* format = "%d%n";
  int std_int, s21_int, std_n, s21_n;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int, &std_n);
  s21_count = s21_sscanf(src, format, &s21_int, &s21_n);
  ck_assert_int_eq(std_int, s21_int);
  ck_assert_int_eq(std_n, s21_n);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_n_empty_string) {
  const char* src = "";
  const char* format = "%n";
  int std_n, s21_n;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_n);
  s21_count = s21_sscanf(src, format, &s21_n);
  ck_assert_int_eq(std_n, s21_n);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

// * - tests for %%

START_TEST(test_sscanf_percent) {
  const char* src = " %; 0xff6732";
  const char* format = " %%;%n";
  int std_int, s21_int;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int);
  s21_count = s21_sscanf(src, format, &s21_int);
  ck_assert_int_eq(std_int, s21_int);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

// * - mix tests

START_TEST(test_sscanf_mix_type_length_width) {
  const char* src =
      "123;4567 Hello worldu|%^ yui067 , 12 (one) 6a 0xf43; 1.753223e2, 45 "
      "6.E-1 6|7. 7.";
  const char* format =
      "%3hd ; %*4d Hello world%c|%%^%3s %*i , %5lu (one)%X%p ;%n %7f %3le, "
      "%o\n %*E %hx |%*g %LG";
  short std_short, s21_short;
  char std_char, s21_char;
  char std_str[4], s21_str[4];
  unsigned long std_ulint, s21_ulint;
  unsigned std_uintx, s21_uintx;
  void *std_ptr, *s21_ptr;
  int std_n, s21_n;
  float std_float, s21_float;
  double std_double, s21_double;
  unsigned std_uinto, s21_uinto;
  unsigned short std_shortx, s21_shortx;
  long double std_ldouble, s21_ldouble;
  int std_count, s21_count;

  std_count = sscanf(src, format, &std_short, &std_char, std_str, &std_ulint,
                     &std_uintx, &std_ptr, &std_n, &std_float, &std_double,
                     &std_uinto, &std_shortx, &std_ldouble);
  s21_count = s21_sscanf(src, format, &s21_short, &s21_char, s21_str,
                         &s21_ulint, &s21_uintx, &s21_ptr, &s21_n, &s21_float,
                         &s21_double, &s21_uinto, &s21_shortx, &s21_ldouble);
  ck_assert_int_eq(std_short, s21_short);
  ck_assert_int_eq(std_char, s21_char);
  ck_assert_str_eq(std_str, s21_str);
  ck_assert_msg(std_ulint == s21_ulint, "Values are not equal: %ld != %ld",
                std_ulint, s21_ulint);
  ck_assert_uint_eq(std_uintx, s21_uintx);
  ck_assert_ptr_eq(std_ptr, s21_ptr);
  ck_assert_int_eq(std_n, s21_n);
  ck_assert_float_eq(std_float, s21_float);
  ck_assert_double_eq(std_double, s21_double);
  ck_assert_uint_eq(std_uinto, s21_uinto);
  ck_assert_uint_eq(std_shortx, s21_shortx);
  ck_assert_ldouble_eq(std_ldouble, s21_ldouble);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_length_int_and_incorrect_percent) {
  const char* src = "7777123  643";
  const char* format = " 7777 %ld %% %ld ";
  long std_lint1, std_lint2 = 1, s21_lint1, s21_lint2 = 1;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_lint1, &std_lint2);
  s21_count = s21_sscanf(src, format, &s21_lint1, &s21_lint2);
  ck_assert_msg(std_lint1 == s21_lint1, "Values are not equal: %ld != %ld",
                std_lint1, s21_lint1);
  ck_assert_msg(std_lint2 == s21_lint2, "Values are not equal: %ld != %ld",
                std_lint2, s21_lint2);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_united_str) {
  const char* src = "123456fhjk 4;5  7 1236756";
  const char* format = "%3d%3d%s 4;5  7 %2d%d";
  int std_int1, std_int2, std_int3, std_int4;
  int s21_int1, s21_int2, s21_int3, s21_int4;
  char std_str[10], s21_str[10];
  int std_count, s21_count;
  std_count =
      sscanf(src, format, &std_int1, &std_int2, std_str, &std_int3, &std_int4);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2, s21_str, &s21_int3,
                         &s21_int4);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_int3, s21_int3);
  ck_assert_int_eq(std_int4, s21_int4);
  ck_assert_str_eq(std_str, s21_str);
}
END_TEST

START_TEST(test_sscanf_percent_sign) {
  const char* src = "%%X";
  const char* format = "%%%c";
  char std_char, s21_char;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_char);
  s21_count = s21_sscanf(src, format, &s21_char);
  ck_assert_int_eq(std_char, s21_char);
  ck_assert_int_eq(std_count, s21_count);
}
END_TEST

START_TEST(test_sscanf_short_variants) {
  const char* src = "32767, -32768, 377; 65535, 0X0X1a3f:  : -1A3F";
  const char* format = "%hd, %hi, %ho; %hu, 0X%hx: : %hX";
  short std_short1 = 0, std_short2 = 0;
  short s21_short1 = 0, s21_short2 = 0;
  unsigned short std_ushorto = 0, std_ushort = 0, std_ushortx1 = 0,
                 std_ushortx2 = 0;
  unsigned short s21_ushorto = 0, s21_ushort = 0, s21_ushortx1 = 0,
                 s21_ushortx2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_short1, &std_short2, &std_ushorto,
                     &std_ushort, &std_ushortx1, &std_ushortx2);
  s21_count = s21_sscanf(src, format, &s21_short1, &s21_short2, &s21_ushorto,
                         &s21_ushort, &s21_ushortx1, &s21_ushortx2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_short1, s21_short1);
  ck_assert_int_eq(std_short2, s21_short2);
  ck_assert_int_eq(std_ushorto, s21_ushorto);
  ck_assert_int_eq(std_ushort, s21_ushort);
  ck_assert_int_eq(std_ushortx1, s21_ushortx1);
  ck_assert_int_eq(std_ushortx2, s21_ushortx2);
}
END_TEST

START_TEST(test_sscanf_long_variants) {
  const char* src =
      "2147483647nan -2147483648inf 377ku 4294967295: 1a3f       ^^ -1A3F";
  const char* format = "%ld nan %liinf %loku %lu: %lx ^^ %lX";
  long std_lint1 = 0, std_lint2 = 0;
  long s21_lint1 = 0, s21_lint2 = 0;
  unsigned long std_ulinto = 0, std_ulint = 0, std_ulintx1 = 0, std_ulintx2 = 0;
  unsigned long s21_ulinto = 0, s21_ulint = 0, s21_ulintx1 = 0, s21_ulintx2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_lint1, &std_lint2, &std_ulinto,
                     &std_ulint, &std_ulintx1, &std_ulintx2);
  s21_count = s21_sscanf(src, format, &s21_lint1, &s21_lint2, &s21_ulinto,
                         &s21_ulint, &s21_ulintx1, &s21_ulintx2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_msg(std_lint1 == s21_lint1, "Values are not equal: %ld != %ld",
                std_lint1, s21_lint1);
  ck_assert_msg(std_lint2 == s21_lint2, "Values are not equal: %ld != %ld",
                std_lint2, s21_lint2);
  ck_assert_msg(std_ulinto == s21_ulinto, "Values are not equal: %ld != %ld",
                std_ulinto, s21_ulinto);
  ck_assert_msg(std_ulint == s21_ulint, "Values are not equal: %ld != %ld",
                std_ulint, s21_ulint);
  ck_assert_msg(std_ulintx1 == s21_ulintx1, "Values are not equal: %ld != %ld",
                std_ulintx1, s21_ulintx1);
  ck_assert_msg(std_ulintx2 == s21_ulintx2, "Values are not equal: %ld != %ld",
                std_ulintx2, s21_ulintx2);
}
END_TEST

START_TEST(test_sscanf_percent2) {
  const char* str = "%%% %%%X";
  const char* format = "%%%c";
  char std_char = 0, s21_char = 0;
  int std_count, s21_count;
  std_count = sscanf(str, format, &std_char);
  s21_count = s21_sscanf(str, format, &s21_char);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char, s21_char);
}
END_TEST

START_TEST(test_sscanf_ret) {
  const char* src = "%1";
  const char* format = "%n%%%*d%d";
  int std_int1, std_int2 = 0, s21_int1, s21_int2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, format, &std_int1, &std_int2);
  s21_count = s21_sscanf(src, format, &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int1, s21_int2);
}
END_TEST

Suite* sscanf_suite(void) {
  Suite* s = suite_create("s21_sscanf");
  TCase* tc_core = tcase_create("Core");

  // Tests %c
  tcase_add_test(tc_core, test_sscanf_char);
  tcase_add_test(tc_core, test_sscanf_char_sep);
  tcase_add_test(tc_core, test_sscanf_char_with_spaces);
  tcase_add_test(tc_core, test_sscanf_char_end_of_string);
  tcase_add_test(tc_core, test_sscanf_char_newline);
  tcase_add_test(tc_core, test_sscanf_char_buf);

  // Tests %d
  tcase_add_test(tc_core, test_sscanf_int);
  tcase_add_test(tc_core, test_sscanf_int_zeros);
  tcase_add_test(tc_core, test_sscanf_int_zero);
  tcase_add_test(tc_core, test_sscanf_int_incorrect_signs);
  tcase_add_test(tc_core, test_sscanf_int_empty_str);
  tcase_add_test(tc_core, test_sscanf_int_incorrect_chars);
  tcase_add_test(tc_core, test_sscanf_int_limits);
  tcase_add_test(tc_core, test_sscanf_d_overflow);

  // Tests %i
  tcase_add_test(tc_core, test_sscanf_i_dec);
  tcase_add_test(tc_core, test_sscanf_i_oct);
  tcase_add_test(tc_core, test_sscanf_i_hex);
  tcase_add_test(tc_core, test_sscanf_i_hex_incorrect);
  tcase_add_test(tc_core, test_sscanf_i_oct_incorrect);
  tcase_add_test(tc_core, test_sscanf_i_octal_incorrect);
  tcase_add_test(tc_core, test_sscanf_i_overflow);

  // Tests %e %E %f %g %G
  tcase_add_test(tc_core, test_sscanf_e);
  tcase_add_test(tc_core, test_sscanf_e_float_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_e_double);
  tcase_add_test(tc_core, test_sscanf_e_long_double);
  tcase_add_test(tc_core, test_sscanf_E);
  tcase_add_test(tc_core, test_sscanf_f);
  tcase_add_test(tc_core, test_sscanf_g);
  tcase_add_test(tc_core, test_sscanf_G);
  tcase_add_test(tc_core, test_sscanf_float_nan_inf);
  tcase_add_test(tc_core, test_sscanf_correct_inf);
  tcase_add_test(tc_core, test_sscanf_correct_inf2);
  tcase_add_test(tc_core, test_sscanf_incorrect_inf);
  tcase_add_test(tc_core, test_sscanf_float_exp_incorrect_width);

  // Tests %o
  tcase_add_test(tc_core, test_sscanf_octal);
  tcase_add_test(tc_core, test_sscanf_o_correct_width);
  tcase_add_test(tc_core, test_sscanf_o_incorrect_width);
  tcase_add_test(tc_core, test_sscanf_o_overflow);

  // Tests %s
  tcase_add_test(tc_core, test_sscanf_str_width);
  tcase_add_test(tc_core, test_sscanf_string_with_asterisk);
  tcase_add_test(tc_core, test_sscanf_leading_space);
  tcase_add_test(tc_core, test_sscanf_string_with_punctuation);
  tcase_add_test(tc_core, test_sscanf_only_spaces);
  tcase_add_test(tc_core, test_sscanf_string_width_1);
  tcase_add_test(tc_core, test_sscanf_width_longer_than_string);

  // Tests %u
  tcase_add_test(tc_core, test_sscanf_uint_max);
  tcase_add_test(tc_core, test_sscanf_u_empty_str);
  tcase_add_test(tc_core, test_sscanf_u_overflow);

  // Tests %x %X
  tcase_add_test(tc_core, test_sscanf_x_correct_width);
  tcase_add_test(tc_core, test_sscanf_x_incorrect_prefix);
  tcase_add_test(tc_core, test_sscanf_x_prefix_width);
  tcase_add_test(tc_core, test_sscanf_x_overflow);

  // Tests %p
  tcase_add_test(tc_core, test_sscanf_p);
  tcase_add_test(tc_core, test_sscanf_p_incorrect);
  tcase_add_test(tc_core, test_sscanf_p_negative);
  tcase_add_test(tc_core, test_sscanf_p_neg_pos);
  tcase_add_test(tc_core, test_sscanf_p_overflow);

  // Tests %n
  tcase_add_test(tc_core, test_sscanf_n);
  tcase_add_test(tc_core, test_sscanf_n_count);
  tcase_add_test(tc_core, test_sscanf_n_empty_string);

  // Tests %%
  tcase_add_test(tc_core, test_sscanf_percent);

  // Mix tests
  tcase_add_test(tc_core, test_sscanf_mix_type_length_width);
  tcase_add_test(tc_core, test_sscanf_length_int_and_incorrect_percent);
  tcase_add_test(tc_core, test_sscanf_united_str);
  tcase_add_test(tc_core, test_sscanf_percent_sign);
  tcase_add_test(tc_core, test_sscanf_all_ascii_chars);
  tcase_add_test(tc_core, test_sscanf_short_variants);
  tcase_add_test(tc_core, test_sscanf_long_variants);
  tcase_add_test(tc_core, test_sscanf_percent2);
  tcase_add_test(tc_core, test_sscanf_ret);
  suite_add_tcase(s, tc_core);
  return s;
}
