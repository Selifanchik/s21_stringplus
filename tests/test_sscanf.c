#include <check.h>
#include <stdio.h>
#include <string.h>

#include "../s21_string.h"

/*
  * + Тесты для спецификатора с (%c)
    * + корректные значения
    * + некоректные значения
    * + корректные и некорректные значения
  * - Тесты для спецификатора d (%d)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора i (%i)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора e (%e)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора E (%E)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора f (%f)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора g (%g)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора G (%G)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора o (%o)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора s (%s)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора u (%u)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора x (%x)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора X (%X)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора p (%p)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора n (%n)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
  * - Тесты для спецификатора % (%%)
    * - корректные значения
    * - некоректные значения
    * - корректные и некорректные значения
*/
 

// * + Тесты для спецификатора c (%c)
  // * + корректные значения
  // * + некоректные значения
  // * + корректные и некорректные значения

// Тест %c
START_TEST(test_sscanf_char) {
  const char *src = "1Hello, world!   ";
  char std_char, s21_char;
  int std_count, s21_count;
  std_count = sscanf(src, "%c", &std_char);
  s21_count = s21_sscanf(src, "%c", &s21_char);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char, s21_char);
}
END_TEST

// Тест %c пробелы впереди
START_TEST(test_sscanf_char_with_spaces) {
  const char *src = "   1Hello, world!   ";
  char std_char1, s21_char1;
  char std_char2, s21_char2;
  int std_count, s21_count;
  std_count = sscanf(src, "%c %c", &std_char1, &std_char2);
  s21_count = s21_sscanf(src, "%c %c", &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST

// Чтение символов с *
START_TEST(test_sscanf_char_asterisk) {
  const char *src = "CD";
  char std_char, s21_char;
  int std_count, s21_count;
  std_count = sscanf(src, "%*c%c", &std_char);
  s21_count = s21_sscanf(src, "%*c%c", &s21_char);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char, s21_char);
}
END_TEST

// Чтение символов с шириной %3c
START_TEST(test_sscanf_repeated_chars) {
  const char *src = "abcde";
  char std_buf[4] = {0}, s21_buf[4] = {0};
  int std_count, s21_count;
  std_count = sscanf(src, "%3c", std_buf);
  s21_count = s21_sscanf(src, "%3c", s21_buf);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_mem_eq(std_buf, s21_buf, 3);
}
END_TEST

// что за тест?
START_TEST(test_sscanf_all_ascii_chars) {
  for (int i = 0; i < 128; ++i) {
    char src[2] = {0};  // строка из одного символа + '\0'
    src[0] = (char)i;

    char std_char, s21_char;
    int std_count, s21_count;
    std_count = sscanf(src, "%c", &std_char);
    s21_count = s21_sscanf(src, "%c", &s21_char);

    ck_assert_msg(std_count == s21_count, "Count mismatch for ASCII %d (0x%02X)", i,
                  i);
    if (std_count == 1 && s21_count == 1) {
      ck_assert_msg(std_char == s21_char,
                    "Char mismatch for ASCII %d (0x%02X): std='%c' s21='%c'", i,
                    i, std_char, s21_char);
    }
  }
}
END_TEST

// Чтение пустой строки
START_TEST(test_sscanf_char_end_of_string) {
  const char *src = "";
  char std_char1 = 0, s21_char1 = 0;
  char std_char2 = 0, s21_char2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, "%c%c", &std_char1, &std_char2);
  s21_count = s21_sscanf(src, "%c%c", &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST

// Чтение 2x символов
START_TEST(test_sscanf_char_multiple) {
  const char *src = "AB";
  char std_char1, std_char2, s21_char1, s21_char2;
  int std_count, s21_count;
  std_count = sscanf(src, "%c%c", &std_char1, &std_char2);
  s21_count = s21_sscanf(src, "%c%c", &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST

//  
START_TEST(test_sscanf_char_width_and_length) {
  const char *src = "CD-DVD";
  wchar_t std_buf[3], s21_buf[3];
  int std_count, s21_count;
  std_count = sscanf(src, "%*c%3lc", std_buf);  // пропускаем первый символ
  s21_count = s21_sscanf(src, "%*c%3lc", s21_buf);
  ck_assert_int_eq(std_count, s21_count);
  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(std_buf[i], s21_buf[i]);
  }
}
END_TEST

// Чтение символа '\n'
START_TEST(test_sscanf_char_newline) {
  const char *src = "123\n";
  char std_char1, s21_char1;
  char std_char2 = 0, s21_char2 = 0;
  int std_count, s21_count;
  std_count = sscanf(src, "%*3c%c %c", &std_char1, &std_char2);
  s21_count = s21_sscanf(src, "%*3c%c %c", &s21_char1, &s21_char2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_char1, s21_char1);
  ck_assert_int_eq(std_char2, s21_char2);
}
END_TEST


  // * - Тесты для спецификатора d (%d)
  //   * - корректные значения
  //   * - некоректные значения
  //   * - корректные и некорректные значения

// Тест %d
START_TEST(test_sscanf_int) {
  const char *src = "   1 ";
  int std_int, s21_int;
  int std_count, s21_count;
  std_count = sscanf(src, "%d", &std_int);
  s21_count = s21_sscanf(src, "%d", &s21_int);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int, s21_int);
}
END_TEST

// Тест %d +-
START_TEST(test_sscanf_int_negative) {
  const char *src = "   -14532 +14532";
  int std_int1, s21_int1;
  int std_int2, s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src, "%d %d", &std_int1, &std_int2);
  s21_count = s21_sscanf(src, "%d %d", &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
}
END_TEST

// Тест %d ведущие нули +-
START_TEST(test_sscanf_int_zeros) {
  const char *src = "0001450 -0001450 +0001450";
  int std_int1, s21_int1;
  int std_int2, s21_int2;
  int std_int3, s21_int3;
  int std_count, s21_count;
  std_count = sscanf(src, "%d\r\v%d%d", &std_int1, &std_int2, &std_int3);
  s21_count = s21_sscanf(src, "%d\r\v%d%d", &s21_int1, &s21_int2, &s21_int3);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_int3, s21_int3);
}
END_TEST

// Тест %d ноль
START_TEST(test_sscanf_int_zero) {
  const char *src = "0 +0; -0f";
  int std_int1, s21_int1;
  int std_int2, s21_int2;
  int std_int3, s21_int3;
  int std_count, s21_count;
  std_count = sscanf(src, "%d\n%d;\t%d", &std_int1, &std_int2, &std_int3);
  s21_count = s21_sscanf(src, "%d\n%d;\t%d", &s21_int1, &s21_int2, &s21_int3);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);
  ck_assert_int_eq(std_int3, s21_int3);
}
END_TEST

// Тест %d ширина
START_TEST(test_sscanf_int_width1) {
  const char *src = "123-14532 3452Abc";
  int std_int1, s21_int1;
  int std_int2, s21_int2;
  int std_count, s21_count;
  std_count = sscanf(src, "%*3d%5d%*d %5d", &std_int1, &std_int2);
  s21_count = s21_sscanf(src, "%*3d%5d%*d %5d", &s21_int1, &s21_int2);
  ck_assert_int_eq(std_count, s21_count);
  ck_assert_int_eq(std_int1, s21_int1);
  ck_assert_int_eq(std_int2, s21_int2);

}
END_TEST



// Тест %d некорректное +-
START_TEST(test_sscanf_int_incorrect_signs) {
  const char *str = "+-23";
  int a = 2;
  int b = 2;
  int count, s21_count;
  count = sscanf(str, "%d", &a);
  s21_count = s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %d некорректное пустая строка
START_TEST(test_sscanf_int_empty_str) {
  const char *str = "";
  int a = 1;
  int b = 1;
  int count, s21_count;
  count = sscanf(str, "%d", &a);
  s21_count = s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %d некорректное символы
START_TEST(test_sscanf_int_incorrect_chars) {
  const char *str = " Hello, world!   ";
  int a = 5;
  int b = 5;
  int count, s21_count;
  count = sscanf(str, "%d", &a);
  s21_count = s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST






// Тест %i dec
START_TEST(test_sscanf_i_dec) {
  const char *str = "   1Hello, world!   ";
  int a;
  int b;
  int count, s21_count;
  count = sscanf(str, "%d", &a);
  s21_count = s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %i oc uncorrect
START_TEST(test_sscanf_i_octal_uncorrect) {
  const char *str = "   Hello, world! 01647 0377   ";
  int a = 0, b = 0;
  int c = 0, d = 0;
  int count, s21_count;
  count = sscanf(str, "%i %i", &a, &b);
  s21_count = s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %i oc
START_TEST(test_sscanf_i_octal) {
  const char *str = "    01647    0377   ";
  int a = 0, b = 0;
  int c = 0, d = 0;
  int count, s21_count;
  count = sscanf(str, "%i %i", &a, &b);
  s21_count = s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %i oc с +- и шириной и делителем
START_TEST(test_sscanf_i_octal_neg_pos_width_delim) {
  const char *str = "    -0377,    +0377,   +0377, +0377";
  int a, b, c = 1, d = 4;
  int e, f, g = 1, h = 4;
  int count, s21_count;
  count = sscanf(str, "%i, %i, %1i, %6i", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%i, %i, %1i, %6i", &e, &f, &g, &h);
  ck_assert_int_eq(a, e);
  ck_assert_int_eq(b, f);
  ck_assert_int_eq(c, g);
  ck_assert_int_eq(d, h);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %i hex
START_TEST(test_sscanf_i_hex) {
  const char *str = " 0x1A3F 0XFF   ";
  int a, b;
  int c, d;
  int count, s21_count;
  count = sscanf(str, "%i %i", &a, &b);
  s21_count = s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %i hex uncorrect
START_TEST(test_sscanf_i_hex_uncorrect) {
  const char *str = "w 0x1A3F 0XFF   ";
  int a = 0, b = 0;
  int c = 0, d = 0;
  int count, s21_count;
  count = sscanf(str, "%i %i", &a, &b);
  s21_count = s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %i hex с +- шириной делителем
START_TEST(test_sscanf_i_hex_neg_pos_width_delim) {
  const char *str = " -0x1A3F +0XFF,-0x1A3F,+0x1A3F ";
  int a, b, c = 1, d = 4;
  int e, f, g = 1, h = 4;
  int count, s21_count;
  count = sscanf(str, "%i %i,%6i, %8i", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%i %i,%6i, %8i", &e, &f, &g, &h);
  ck_assert_int_eq(a, e);
  ck_assert_int_eq(b, f);
  ck_assert_int_eq(c, g);
  ck_assert_int_eq(d, h);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %e float
START_TEST(test_sscanf_e_float) {
  const char *str = ".3e2 3e2 4e-3 1.234";
  float a, b, c, d;
  float e, f, g, h;
  int count, s21_count;
  count = sscanf(str, "%e %e %e %e", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%e %e %e %e", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %e float +- delim width uncorrect
START_TEST(test_sscanf_e_float_neg_pos_width_delim) {
  const char *str = "\t+.3e2 -3e2: 0.0e0 \n1.234s 0.ee2";
  float a, b, c, d, u = 1;
  float e, f, g, h, u1 = 1;
  int count, s21_count;
  count = sscanf(str, "%e %e: %e %5e %e", &a, &b, &c, &d, &u);
  s21_count = s21_sscanf(str, "%e %e: %e %5e %e", &e, &f, &g, &h, &u1);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
  ck_assert_float_eq(u, u1);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %e double
START_TEST(test_sscanf_e_double) {
  const char *str = ".3e8 3e7 4e-5 455.2345653";
  double a, b, c, d;
  double e, f, g, h;
  int count, s21_count;
  count = sscanf(str, "%le %le %le %le", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%le %le %le %le", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %e double
START_TEST(test_sscanf_e_long_double) {
  const char *str = ".3e10 3e11 4e-9 455.234565651";
  long double a, b, c, d;
  long double e, f, g, h;
  int count, s21_count;
  count = sscanf(str, "%Le %Le %Le %Le", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%Le %Le %Le %Le", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %E %f %g %G
START_TEST(test_sscanf_E_f_g_G) {
  const char *str = ".3e2 3e2 4e-3 1.234";
  float a, b, c, d;
  float a1, b1, c1, d1;
  float a2, b2, c2, d2;
  float a3, b3, c3, d3;
  float e, f, g, h;
  float e1, f1, g1, h1;
  float e2, f2, g2, h2;
  float e3, f3, g3, h3;
  int count, s21_count;
  count = sscanf(str, "%E %E %E %E", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%E %E %E %E", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
  ck_assert_int_eq(count, s21_count);
  count = sscanf(str, "%f %f %f %f", &a1, &b1, &c1, &d1);
  s21_count = s21_sscanf(str, "%f %f %f %f", &e1, &f1, &g1, &h1);
  ck_assert_float_eq(a1, e1);
  ck_assert_float_eq(b1, f1);
  ck_assert_float_eq(c1, g1);
  ck_assert_float_eq(d1, h1);
  ck_assert_int_eq(count, s21_count);
  count = sscanf(str, "%g %g %g %g", &a2, &b2, &c2, &d2);
  s21_count = s21_sscanf(str, "%g %g %g %g", &e2, &f2, &g2, &h2);
  ck_assert_float_eq(a2, e2);
  ck_assert_float_eq(b2, f2);
  ck_assert_float_eq(c2, g2);
  ck_assert_float_eq(d2, h2);
  ck_assert_int_eq(count, s21_count);
  count = sscanf(str, "%G %G %G %G", &a3, &b3, &c3, &d3);
  s21_count = s21_sscanf(str, "%G %G %G %G", &e3, &f3, &g3, &h3);
  ck_assert_float_eq(a3, e3);
  ck_assert_float_eq(b3, f3);
  ck_assert_float_eq(c3, g3);
  ck_assert_float_eq(d3, h3);
}
END_TEST

// Тест %o
START_TEST(test_sscanf_octal) {
  const char *str = "0567 0611";
  unsigned int a, b;
  unsigned int c, d;
  int count, s21_count;
  count = sscanf(str, "%o %o", &a, &b);
  s21_count = s21_sscanf(str, "%o %o", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %s
START_TEST(test_sscanf_str_width) {
  const char *str = "   1Hello, world! ";
  char a[20];
  char b[20];
  int count, s21_count;
  count = sscanf(str, "%19s", a);
  s21_count = s21_sscanf(str, "%19s", b);
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_string_with_asterisk) {
  const char *str = "skip keep";
  char a[100] = {0}, b[100] = {0};
  int c1 = sscanf(str, "%*s %s", a);
  int c2 = s21_sscanf(str, "%*s %s", b);
  ck_assert_int_eq(c1, c2);
  ck_assert_str_eq(a, b);  // Ожидаем "keep"
}
END_TEST

START_TEST(test_sscanf_leading_space) {
  const char *str = "     world";
  char a[10];
  char b[10];
  int c1 = sscanf(str, "%s", a);
  int c2 = s21_sscanf(str, "%s", b);
  ck_assert_int_eq(c1, c2);
  ck_assert_str_eq(a, b);  // Ожидаем "word"
}
END_TEST

START_TEST(test_sscanf_string_with_punctuation) {
  const char *str = "text,more";
  char a[10];
  char b[10];
  int c1 = sscanf(str, "%s", a);
  int c2 = s21_sscanf(str, "%s", b);
  ck_assert_int_eq(c1, c2);
  ck_assert_str_eq(a, b);  // Ожидаем "text,more"
}
END_TEST

START_TEST(test_sscanf_only_spaces) {
  const char *str = "   ";
  char a[10];
  char b[10];
  int c1 = sscanf(str, "%s", a);
  int c2 = s21_sscanf(str, "%s", b);
  ck_assert_int_eq(c1, c2);  // Ожидаем 0
}
END_TEST

START_TEST(test_sscanf_string_width_1) {
  const char *str = "abc";
  char a[100] = {0}, b[100] = {0};
  int c1 = sscanf(str, "%1s", a);
  int c2 = s21_sscanf(str, "%1s", b);
  ck_assert_int_eq(c1, c2);
  ck_assert_str_eq(a, b);  // Ожидаем "a"
}
END_TEST

START_TEST(test_sscanf_width_longer_than_string) {
  const char *str = "short";
  char a[100] = {0}, b[100] = {0};
  int c1 = sscanf(str, "%10s", a);
  int c2 = s21_sscanf(str, "%10s", b);
  ck_assert_int_eq(c1, c2);
  ck_assert_str_eq(a, b);  // Ожидаем "short"
}
END_TEST

// Тест %p
START_TEST(test_sscanf_p) {
  void *ptr_sscanf = NULL;
  void *ptr_s21_sscanf = S21_NULL;
  const char *str = " ; 0xff6732";
  int count, s21_count;
  count = sscanf(str, " ;%p", &ptr_sscanf);
  s21_count = s21_sscanf(str, " ;%p", &ptr_s21_sscanf);
  ck_assert_ptr_eq(ptr_sscanf, ptr_s21_sscanf);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_n) {
  int n_sscanf;
  int n_s21_sscanf;
  const char *str = " ; 0xff6732";
  int count, s21_count;
  count = sscanf(str, " ;%n", &n_sscanf);
  s21_count = s21_sscanf(str, " ;%n", &n_s21_sscanf);
  ck_assert_int_eq(n_sscanf, n_s21_sscanf);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_percent) {
  int a;
  int b;
  const char *str = " %; 0xff6732";
  int count, s21_count;
  count = sscanf(str, " %%;%n", &a);
  s21_count = s21_sscanf(str, " %%;%n", &b);
  ck_assert_int_eq(a, a);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_mix_type_length_width) {
  short sh_num, s21_sh_num;
  char symb, s21_symb;
  char str[100], s21_str[100];
  unsigned long u_l_num, s21_u_l_num;
  unsigned u_x_num, s21_u_x_num;
  void *ptr, *s21_ptr;
  int n, s21_n;
  float f_num, s21_f_num;
  double e_num, s21_e_num;
  unsigned o_num, s21_o_num;
  unsigned short x_num, s21_x_num;
  long double g_num, s21_g_num;
  int count, s21_count;

  count = sscanf(
      "123;4567 Hello worldu|%^ yui067 , 12 (one) 6a 0xf43; 1.753223e2, 45 "
      "6.E-1 6|7. 7.",
      "%3hd ; %*4d Hello world%c|%%^%3s %*i , %5lu (one)%X%p ;%n %7f %3le, "
      "%o\n %*E %hx |%*g %LG",
      &sh_num, &symb, str, &u_l_num, &u_x_num, &ptr, &n, &f_num, &e_num, &o_num,
      &x_num, &g_num);
  s21_count = s21_sscanf(
      "123;4567 Hello worldu|%^ yui067 , 12 (one) 6a 0xf43; 1.753223e2, 45 "
      "6.E-1 6|7. 7.",
      "%3hd ; %*4d Hello world%c|%%^%3s %*i , %5lu (one)%X%p ;%n %7f %3le, "
      "%o\n %*E %hx |%*g %LG",
      &s21_sh_num, &s21_symb, s21_str, &s21_u_l_num, &s21_u_x_num, &s21_ptr,
      &s21_n, &s21_f_num, &s21_e_num, &s21_o_num, &s21_x_num, &s21_g_num);
  ck_assert_int_eq(sh_num, s21_sh_num);
  ck_assert_int_eq(symb, s21_symb);
  ck_assert_str_eq(str, s21_str);
  ck_assert_uint_eq(u_l_num, s21_u_l_num);
  ck_assert_uint_eq(u_x_num, s21_u_x_num);
  ck_assert_ptr_eq(ptr, s21_ptr);
  ck_assert_int_eq(n, s21_n);
  ck_assert_float_eq(f_num, s21_f_num);
  ck_assert_double_eq(e_num, s21_e_num);
  ck_assert_uint_eq(o_num, s21_o_num);
  ck_assert_uint_eq(x_num, s21_x_num);
  ck_assert_ldouble_eq(g_num, s21_g_num);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_o_correct_width) {
  unsigned a, b, c = 1, d = 1;
  int count, s21_count;
  count = sscanf(";54   0123", ";%o %1o", &a, &c);
  s21_count = s21_sscanf(";54   0123", ";%o %1o", &b, &d);
  ck_assert_uint_eq(a, b);
  ck_assert_uint_eq(c, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_o_uncorrect_width) {
  unsigned a, b, c = 1, d = 1;
  int count, s21_count;
  count = sscanf(";54   90123", ";%o %1o", &a, &c);
  s21_count = s21_sscanf(";54   90123", ";%o %1o", &b, &d);
  ck_assert_uint_eq(a, b);
  ck_assert_uint_eq(c, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_x_correct_width) {
  unsigned a, b, c = 1, d = 1;
  int count, s21_count;
  count = sscanf(";54   0x123", ";%x %1x", &a, &c);
  s21_count = s21_sscanf(";54   0x123", ";%x %1x", &b, &d);
  ck_assert_uint_eq(a, b);
  ck_assert_uint_eq(c, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_x_incorrect_width) {
  unsigned a = 1, b = 1;
  int count, s21_count;
  count = sscanf(";g0x123", ";%x", &a);
  s21_count = s21_sscanf(";g0x123", ";%x", &b);
  ck_assert_uint_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_length_int_and_incorrect_percent) {
  long a, b = 1, c, d = 1;
  int count, s21_count;
  count = sscanf("123  643", "  %ld %% %ld ", &a, &b);
  s21_count = s21_sscanf("123  643", "  %ld %% %ld ", &c, &d);
  ck_assert_uint_eq(a, c);
  ck_assert_uint_eq(b, d);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_p_incorrect) {
  void *ptr = NULL, *s21_ptr = S21_NULL;
  int count, s21_count;
  count = sscanf("qwe123  643", "%p", &ptr);
  s21_count = s21_sscanf("qwe123  643", "%p", &s21_ptr);
  ck_assert_ptr_eq(ptr, s21_ptr);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

START_TEST(test_sscanf_wchar_string) {
  wchar_t symb[10] = {0}, s21_symb[10] = {0};
  int count, s21_count;
  count = sscanf("qwe123  643", "qw%2ls", symb);
  s21_count = s21_sscanf("qwe123  643", "qw%2ls", s21_symb);
  ck_assert_int_eq(wcscmp(symb, s21_symb), 0);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест %E %f nan
START_TEST(test_sscanf_float_nan_inf) {
  const char *str = "nan inf; .8[1.";
  long double a, b, c, d;
  long double e, f, g, h;
  int count, s21_count;
  count = sscanf(str, "%Lf %Lf; %Lf [ %Lf", &a, &b, &c, &d);
  s21_count = s21_sscanf(str, "%Lf %Lf; %Lf [ %Lf", &e, &f, &g, &h);
  ck_assert_ldouble_nan(a);
  ck_assert_ldouble_nan(e);
  ck_assert_ldouble_infinite(b);
  ck_assert_ldouble_infinite(f);
  ck_assert_ldouble_eq(c, g);
  ck_assert_ldouble_eq(d, h);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Тест склееное число
START_TEST(test_sscanf_united_str) {
  const char str[] = "123456fhjk 1236756";
  int a, b, c, d;
  int e, f, g, h;
  char s1[10];
  char s2[10];
  int s21_count = s21_sscanf(str, "%3d%3d%s %2d%d", &a, &b, s1, &c, &d);
  int count = sscanf(str, "%3d%3d%s %2d%d", &e, &f, s2, &g, &h);
  ck_assert_int_eq(count, s21_count);
  ck_assert_int_eq(a, e);
  ck_assert_int_eq(b, f);
  ck_assert_int_eq(c, g);
  ck_assert_int_eq(d, h);
  ck_assert_str_eq(s1, s2);
}
END_TEST

// Чтение int max и int min
START_TEST(test_sscanf_int_limits) {
  int a = 0, b = 0;
  int ret_std = sscanf("2147483647", "%d", &a);
  int ret_s21 = s21_sscanf("2147483647", "%d", &b);
  ck_assert_int_eq(ret_std, ret_s21);
  ck_assert_int_eq(a, b);

  ret_std = sscanf("-2147483648", "%d", &a);
  ret_s21 = s21_sscanf("-2147483648", "%d", &b);
  ck_assert_int_eq(ret_std, ret_s21);
  ck_assert_int_eq(a, b);
}
END_TEST

// максимальное беззнаковое
START_TEST(test_sscanf_uint_max) {
  unsigned int a, b;
  int count = sscanf("4294967295", "%u", &a);
  int s21_count = s21_sscanf("4294967295", "%u", &b);
  ck_assert_uint_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// тест для n
START_TEST(test_sscanf_n_count) {
  int n1, n2, a, b;
  int count = sscanf("12345", "%d%n", &a, &n1);
  int s21_count = s21_sscanf("12345", "%d%n", &b, &n2);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// тест для %
START_TEST(test_sscanf_percent_sign) {
  char a, b;
  int count = sscanf("%%X", "%%%c", &a);  // Ожидается символ 'X'
  int s21_count = s21_sscanf("%%X", "%%%c", &b);
  ck_assert_int_eq(a, b);
  ck_assert_int_eq(count, s21_count);
}
END_TEST

// Чтение числа с ведущими нулями
START_TEST(test_sscanf_leading_zeros) {
  const char *str = "0000123";
  int a = 0, b = 0;
  int ret_std = sscanf(str, "%d", &a);
  int ret_s21 = s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(ret_std, ret_s21);
  ck_assert_int_eq(a, b);
}
END_TEST

// Тест для short
START_TEST(test_sscanf_short_variants) {
  const char *str = "32767 -32768 377 65535 1a3f 1A3F";

  short int d1_std = 0, d2_std = 0;
  unsigned short o_std = 0, u_std = 0, x_std = 0, X_std = 0;

  int ret_std = sscanf(str, "%hd %hi %ho %hu %hx %hX", &d1_std, &d2_std, &o_std,
                       &u_std, &x_std, &X_std);

  short int d1_s21 = 0, d2_s21 = 0;
  unsigned short o_s21 = 0, u_s21 = 0, x_s21 = 0, X_s21 = 0;
  int ret_s21 = s21_sscanf(str, "%hd %hi %ho %hu %hx %hX", &d1_s21, &d2_s21,
                           &o_s21, &u_s21, &x_s21, &X_s21);

  ck_assert_int_eq(ret_std, ret_s21);

  ck_assert_int_eq(d1_std, d1_s21);
  ck_assert_int_eq(d2_std, d2_s21);
  ck_assert_int_eq(o_std, o_s21);
  ck_assert_int_eq(u_std, u_s21);
  ck_assert_int_eq(x_std, x_s21);
  ck_assert_int_eq(X_std, X_s21);
}
END_TEST

// Тест для long
START_TEST(test_sscanf_long_variants) {
  const char *str = "2147483647 -2147483648 377 4294967295 1a3f 1A3F";

  long int d1_std = 0, d2_std = 0;
  unsigned long o_std = 0, u_std = 0, x_std = 0, X_std = 0;

  int ret_std = sscanf(str, "%ld %li %lo %lu %lx %lX", &d1_std, &d2_std, &o_std,
                       &u_std, &x_std, &X_std);

  long int d1_s21 = 0, d2_s21 = 0;
  unsigned long o_s21 = 0, u_s21 = 0, x_s21 = 0, X_s21 = 0;
  int ret_s21 = s21_sscanf(str, "%ld %li %lo %lu %lx %lX", &d1_s21, &d2_s21,
                           &o_s21, &u_s21, &x_s21, &X_s21);

  ck_assert_int_eq(ret_std, ret_s21);

  ck_assert_int_eq(d1_std, d1_s21);
  ck_assert_int_eq(d2_std, d2_s21);
  ck_assert_int_eq(o_std, o_s21);
  ck_assert_int_eq(u_std, u_s21);
  ck_assert_int_eq(x_std, x_s21);
  ck_assert_int_eq(X_std, X_s21);
}
END_TEST

START_TEST(test_sscanf_percent2) {
  const char *str = "%%% %%%X";
  char a = 0;
  char b = 0;
  int count_std = sscanf(str, "%%%c", &a);
  int count_s21 = s21_sscanf(str, "%%%c", &b);
  ck_assert_int_eq(count_std, count_s21);
  ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_correct_inf) {
  const char *str = "-inf inf infi nan";
  long double a, b, c, e, f, g;
  float d, h;
  char str1[2], str2[2];
  int count_std = sscanf(str, "%Lf %Lf %3Lf %s %f", &a, &b, &c, str1, &d);
  int count_s21 = s21_sscanf(str, "%Lf %Lf %3Lf %s %f", &e, &f, &g, str2, &h);
  ck_assert_int_eq(count_std, count_s21);
  ck_assert_ldouble_infinite(a);
  ck_assert_ldouble_infinite(e);
  ck_assert_ldouble_infinite(b);
  ck_assert_ldouble_infinite(f);
  ck_assert_ldouble_infinite(c);
  ck_assert_ldouble_infinite(g);
  ck_assert_str_eq(str1, str2);
  ck_assert_float_nan(d);
  ck_assert_float_nan(h);
}
END_TEST

START_TEST(test_sscanf_correct_inf2) {
  const char *str = "-iNf infInity naN";
  long double a, b, c, e, f, g;
  int count_std = sscanf(str, "%Lf %Lf %Lf", &a, &b, &c);
  int count_s21 = s21_sscanf(str, "%Lf %Lf %Lf", &e, &f, &g);
  ck_assert_int_eq(count_std, count_s21);
  ck_assert_ldouble_infinite(a);
  ck_assert_ldouble_infinite(e);
  ck_assert_ldouble_infinite(b);
  ck_assert_ldouble_infinite(f);
  ck_assert_ldouble_nan(c);
  ck_assert_ldouble_nan(g);
}
END_TEST

START_TEST(test_sscanf_incorrect_inf) {
  const char *str = "-iNfin inf";
  double a = 0, b = 0;
  char str1[10] = {'\0'}, str2[10] = {'\0'};
  int count_std = sscanf(str, "%lf %s", &a, str1);
  int count_s21 = s21_sscanf(str, "%lf %s", &b, str2);
  ck_assert_int_eq(count_std, count_s21);
  ck_assert_double_eq(a, b);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_sscanf_ret) {
  const char *str = "%1";
  int a, b = 0, c, d = 0;
  int count = sscanf(str, "%n%%%*d%d", &a, &b);
  int s21_count = s21_sscanf(str, "%n%%%*d%d", &c, &d);
  ck_assert_int_eq(count, s21_count);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
}
END_TEST

Suite *sscanf_suite(void) {
  Suite *s = suite_create("s21_sscanf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sscanf_char);
  tcase_add_test(tc_core, test_sscanf_char_with_spaces);
  tcase_add_test(tc_core, test_sscanf_char_asterisk);
  tcase_add_test(tc_core, test_sscanf_char_newline);
  tcase_add_test(tc_core, test_sscanf_char_multiple);
  tcase_add_test(tc_core, test_sscanf_char_end_of_string);
  tcase_add_test(tc_core, test_sscanf_char_width_and_length);
  tcase_add_test(tc_core, test_sscanf_int);
  tcase_add_test(tc_core, test_sscanf_int_negative);
  tcase_add_test(tc_core, test_sscanf_int_incorrect_chars);
  tcase_add_test(tc_core, test_sscanf_int_incorrect_signs);
  tcase_add_test(tc_core, test_sscanf_int_empty_str);
  tcase_add_test(tc_core, test_sscanf_int_zeros);
  tcase_add_test(tc_core, test_sscanf_int_zero);
  tcase_add_test(tc_core, test_sscanf_int_width1);
  tcase_add_test(tc_core, test_sscanf_int_width1);
  tcase_add_test(tc_core, test_sscanf_i_dec);
  tcase_add_test(tc_core, test_sscanf_i_octal);
  tcase_add_test(tc_core, test_sscanf_i_octal_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_i_octal_uncorrect);
  tcase_add_test(tc_core, test_sscanf_i_hex);
  tcase_add_test(tc_core, test_sscanf_i_hex_uncorrect);
  tcase_add_test(tc_core, test_sscanf_i_hex_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_e_float);
  tcase_add_test(tc_core, test_sscanf_e_float_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_e_double);
  tcase_add_test(tc_core, test_sscanf_e_long_double);
  tcase_add_test(tc_core, test_sscanf_E_f_g_G);
  tcase_add_test(tc_core, test_sscanf_octal);
  tcase_add_test(tc_core, test_sscanf_str_width);
  tcase_add_test(tc_core, test_sscanf_string_with_asterisk);
  tcase_add_test(tc_core, test_sscanf_leading_space);
  tcase_add_test(tc_core, test_sscanf_string_with_punctuation);
  tcase_add_test(tc_core, test_sscanf_only_spaces);
  tcase_add_test(tc_core, test_sscanf_string_width_1);
  tcase_add_test(tc_core, test_sscanf_width_longer_than_string);
  tcase_add_test(tc_core, test_sscanf_p);
  tcase_add_test(tc_core, test_sscanf_n);
  tcase_add_test(tc_core, test_sscanf_percent);
  tcase_add_test(tc_core, test_sscanf_mix_type_length_width);
  tcase_add_test(tc_core, test_sscanf_o_correct_width);
  tcase_add_test(tc_core, test_sscanf_x_correct_width);
  tcase_add_test(tc_core, test_sscanf_x_incorrect_width);
  tcase_add_test(tc_core, test_sscanf_length_int_and_incorrect_percent);
  tcase_add_test(tc_core, test_sscanf_p_incorrect);
  tcase_add_test(tc_core, test_sscanf_wchar_string);
  tcase_add_test(tc_core, test_sscanf_float_nan_inf);
  tcase_add_test(tc_core, test_sscanf_united_str);
  tcase_add_test(tc_core, test_sscanf_int_limits);
  tcase_add_test(tc_core, test_sscanf_uint_max);
  tcase_add_test(tc_core, test_sscanf_n_count);
  tcase_add_test(tc_core, test_sscanf_percent_sign);
  tcase_add_test(tc_core, test_sscanf_leading_zeros);
  tcase_add_test(tc_core, test_sscanf_repeated_chars);
  tcase_add_test(tc_core, test_sscanf_all_ascii_chars);
  tcase_add_test(tc_core, test_sscanf_short_variants);
  tcase_add_test(tc_core, test_sscanf_long_variants);
  tcase_add_test(tc_core, test_sscanf_percent2);
  tcase_add_test(tc_core, test_sscanf_correct_inf);
  tcase_add_test(tc_core, test_sscanf_correct_inf2);
  tcase_add_test(tc_core, test_sscanf_incorrect_inf);
  tcase_add_test(tc_core, test_sscanf_ret);
  tcase_add_test(tc_core, test_sscanf_o_uncorrect_width);
  suite_add_tcase(s, tc_core);
  return s;
}
