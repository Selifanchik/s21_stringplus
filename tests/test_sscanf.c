#include <check.h>
#include <stdio.h>
#include <string.h>

#include "../s21_string.h"

// Тест %d
START_TEST(test_sscanf_int) {
  const char *str = "   1 Hello, world!   ";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d отрицательное
START_TEST(test_sscanf_int_negative) {
  const char *str = "   -14532 Hello, world!   ";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d положительное
START_TEST(test_sscanf_int_positive) {
  const char *str = "   +14532 Hello, world!   ";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// // Тест %d некорректное символы
// START_TEST(test_sscanf_int_incorrect_chars) {
//   const char *str = " Hello, world!   ";
//   int a = 5;
//   int b = 5;
//   sscanf(str, "%d", &a);
//   s21_sscanf(str, "%d", &b);
//   ck_assert_int_eq(a, b);
//   //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
// }
// END_TEST

// // Тест %d некорректное +-
// START_TEST(test_sscanf_int_incorrect_signs) {
//   const char *str = "+-23";
//   int a = 2;
//   int b = 2;
//   sscanf(str, "%d", &a);
//   s21_sscanf(str, "%d", &b);
//   ck_assert_int_eq(a, b);
//   //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
// }
// END_TEST

// // Тест %d некорректное пустая строка
// START_TEST(test_sscanf_int_empty_str) {
//   const char *str = "";
//   int a = 1;
//   int b = 1;
//   sscanf(str, "%d", &a);
//   s21_sscanf(str, "%d", &b);
//   ck_assert_int_eq(a, b);
//   //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
// }
// END_TEST

// Тест %d ведущие нули
START_TEST(test_sscanf_int_zeros) {
  const char *str = "0001450";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d отрицательное и нули
START_TEST(test_sscanf_int_negative_and_zeros) {
  const char *str = "-0001450";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d отрицательное и нули
START_TEST(test_sscanf_int_positive_and_zeros) {
  const char *str = "+0001450";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ноль
START_TEST(test_sscanf_int_zero) {
  const char *str = "0 f";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ноль с плюсом
START_TEST(test_sscanf_int_positive_zero) {
  const char *str = "+0 f";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ноль сминусом
START_TEST(test_sscanf_int_negative_zero) {
  const char *str = "-0 f";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d несколько чисел
START_TEST(test_sscanf_int_many_int) {
  const char *str = " 123\t567 2\n\r\v89";
  int a, b, c , d;
  int e, f, g , h;
  sscanf(str, "%d %d %d %d", &a, &b, &c, &d);
  s21_sscanf(str, "%d %d %d %d", &e, &f, &g, &h);
  ck_assert_int_eq(a, e);
  ck_assert_int_eq(b, f);
  ck_assert_int_eq(c, g);
  ck_assert_int_eq(d, h);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ширина
START_TEST(test_sscanf_int_width1) {
  const char *str = "-14532 Hello, world!   ";
  int a;
  int b;
  sscanf(str, "%5d", &a);
  s21_sscanf(str, "%5d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ширина
START_TEST(test_sscanf_int_width2) {
  const char *str = "1453";
  int a;
  int b;
  sscanf(str, "%5d", &a);
  s21_sscanf(str, "%5d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ширина
START_TEST(test_sscanf_int_width3) {
  const char *str = "1453";
  int a;
  int b;
  sscanf(str, "%5d", &a);
  s21_sscanf(str, "%5d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %d ширина
START_TEST(test_sscanf_int_width4) {
  const char *str = "+1453";
  int a;
  int b;
  sscanf(str, "%4d", &a);
  s21_sscanf(str, "%4d", &b);
  ck_assert_int_eq(a, b);
  //!!!!!!!!!!!!!!! еще нужно протестировать результат работы самой функции, успешно записанные данные
}
END_TEST

// Тест %c 
START_TEST(test_sscanf_char) {
  const char *str = "1Hello, world!   ";
  char a;
  char b;
  sscanf(str, "%c", &a);
  s21_sscanf(str, "%c", &b);
  ck_assert_int_eq(a, b);
}
END_TEST

// // Тест %c пробелы впереди
// START_TEST(test_sscanf_char_with_spaces) {
//   const char *str = "   1Hello, world!   ";
//   char a;
//   char b;
//   sscanf(str, "%c", &a);
//   s21_sscanf(str, "%c", &b);
//   ck_assert_int_eq(a, b);
// }
// END_TEST

// Тест %i dec
START_TEST(test_sscanf_i_dec) {
  const char *str = "   1Hello, world!   ";
  int a;
  int b;
  sscanf(str, "%d", &a);
  s21_sscanf(str, "%d", &b);
  ck_assert_int_eq(a, b);
}
END_TEST

// Тест %i oc uncorrect
START_TEST(test_sscanf_i_octal_uncorrect) {
 const char *str = "   Hello, world! 01647 0377   ";
  int a = 0 , b = 0;
  int c = 0, d = 0;
  sscanf(str, "%i %i", &a, &b);
  s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
}
END_TEST

// Тест %i oc
START_TEST(test_sscanf_i_octal) {
 const char *str = "    01647    0377   ";
  int a = 0 , b = 0;
  int c = 0, d = 0;
  sscanf(str, "%i %i", &a, &b);
  s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
}
END_TEST

// // Тест %i oc с +- и шириной и делителем
// START_TEST(test_sscanf_i_octal_neg_pos_width_delim) {
//  const char *str = "    -0377,    +0377,   +0377, +0377";
//   int a, b, c=1, d=4;
//   int e, f, g=1, h=4;
//   sscanf(str, "%i, %i, %1i, %6i", &a, &b, &c, &d);
//   s21_sscanf(str, "%i, %i, %1i, %6i", &e, &f, &g, &h);
//   ck_assert_int_eq(a, e);
//   ck_assert_int_eq(b, f);
//   ck_assert_int_eq(c, g);
//   ck_assert_int_eq(d, h);
// }
// END_TEST

// Тест %i hex
START_TEST(test_sscanf_i_hex) {
  const char *str = " 0x1A3F 0XFF   ";
  int a, b;
  int c, d;
  sscanf(str, "%i %i", &a, &b);
  s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
}
END_TEST

// Тест %i hex uncorrect
START_TEST(test_sscanf_i_hex_uncorrect) {
  const char *str = "w 0x1A3F 0XFF   ";
  int a = 0, b = 0;
  int c = 0, d = 0;
  sscanf(str, "%i %i", &a, &b);
  s21_sscanf(str, "%i %i", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
}
END_TEST

// // Тест %i hex с +- шириной делителем
// START_TEST(test_sscanf_i_hex_neg_pos_width_delim) {
//   const char *str = " -0x1A3F +0XFF,-0x1A3F,+0x1A3F ";
//   int a, b, c=1, d=4;
//   int e, f, g=1, h=4;
//   sscanf(str, "%i %i,%6i, %8i", &a, &b, &c, &d);
//   s21_sscanf(str, "%i %i,%6i, %8i", &e, &f, &g, &h);
//   ck_assert_int_eq(a, e);
//   ck_assert_int_eq(b, f);
//   ck_assert_int_eq(c, g);
//   ck_assert_int_eq(d, h);
// }
// END_TEST

// Тест %e float
START_TEST(test_sscanf_e_float) {
  const char *str = ".3e2 3e2 4e-3 1.234";
  float a, b, c, d;
  float e, f, g, h;
  sscanf(str, "%e %e %e %e", &a, &b, &c, &d);
  s21_sscanf(str, "%e %e %e %e", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
}
END_TEST

// // Тест %e float +- delim width uncorrect
// START_TEST(test_sscanf_e_float_neg_pos_width_delim) {
//   const char *str = "\t+.3e2 -3e2: 0.0e0 \n1.234s 0.ee2";
//   float a, b, c, d, u=1;
//   float e, f, g, h, u1=1;
//   sscanf(str, "%e %e: %e %5e %e", &a, &b, &c, &d, &u);
//   s21_sscanf(str, "%e %e: %e %5e %e", &e, &f, &g, &h, &u1);
//   ck_assert_float_eq(a, e);
//   ck_assert_float_eq(b, f);
//   ck_assert_float_eq(c, g);
//   ck_assert_float_eq(d, h);
//   ck_assert_float_eq(u, u1);
// }
// END_TEST

// Тест %e double
START_TEST(test_sscanf_e_double) {
  const char *str = ".3e8 3e7 4e-5 455.2345653";
  double a, b, c, d;
  double e, f, g, h;
  sscanf(str, "%le %le %le %le", &a, &b, &c, &d);
  s21_sscanf(str, "%le %le %le %le", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
}
END_TEST


// Тест %e double
START_TEST(test_sscanf_e_long_double) {
  const char *str = ".3e10 3e11 4e-9 455.234565651";
  long double a, b, c, d;
  long double e, f, g, h;
  sscanf(str, "%Le %Le %Le %Le", &a, &b, &c, &d);
  s21_sscanf(str, "%Le %Le %Le %Le", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
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
  sscanf(str, "%E %E %E %E", &a, &b, &c, &d);
  s21_sscanf(str, "%E %E %E %E", &e, &f, &g, &h);
  ck_assert_float_eq(a, e);
  ck_assert_float_eq(b, f);
  ck_assert_float_eq(c, g);
  ck_assert_float_eq(d, h);
  sscanf(str, "%f %f %f %f", &a1, &b1, &c1, &d1);
  s21_sscanf(str, "%f %f %f %f", &e1, &f1, &g1, &h1);
  ck_assert_float_eq(a1, e1);
  ck_assert_float_eq(b1, f1);
  ck_assert_float_eq(c1, g1);
  ck_assert_float_eq(d1, h1);
  sscanf(str, "%g %g %g %g", &a2, &b2, &c2, &d2);
  s21_sscanf(str, "%g %g %g %g", &e2, &f2, &g2, &h2);
  ck_assert_float_eq(a2, e2);
  ck_assert_float_eq(b2, f2);
  ck_assert_float_eq(c2, g2);
  ck_assert_float_eq(d2, h2);
  sscanf(str, "%G %G %G %G", &a3, &b3, &c3, &d3);
  s21_sscanf(str, "%G %G %G %G", &e3, &f3, &g3, &h3);
  ck_assert_float_eq(a3, e3);
  ck_assert_float_eq(b3, f3);
  ck_assert_float_eq(c3, g3);
  ck_assert_float_eq(d3, h3);
}
END_TEST

// Тест %o
START_TEST(test_sscanf_octal) {
  const char *str = "0567 0611";
  unsigned int a,b;
  unsigned int c,d;
  sscanf(str, "%o %o", &a, &b);
  s21_sscanf(str, "%o %o", &c, &d);
  ck_assert_int_eq(a, c);
  ck_assert_int_eq(b, d);
}
END_TEST

// // Тест %s
// START_TEST(test_sscanf_str) {
//   const char *str = "   1Hello, world! ";
//   char a[20];
//   char b[20];
//   sscanf(str, "%s", a);
//   s21_sscanf(str, "%s", b);
//   ck_assert_str_eq(a, b);
// }
// END_TEST

Suite *sscanf_suite(void) {
  Suite *s = suite_create("s21_sscanf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sscanf_int);
  tcase_add_test(tc_core, test_sscanf_int_negative);
  tcase_add_test(tc_core, test_sscanf_int_positive);
  //tcase_add_test(tc_core, test_sscanf_int_incorrect_chars);
  //tcase_add_test(tc_core, test_sscanf_int_incorrect_signs);
  //tcase_add_test(tc_core, test_sscanf_int_empty_str);
  tcase_add_test(tc_core, test_sscanf_int_zeros);
  tcase_add_test(tc_core, test_sscanf_int_negative_and_zeros);
  tcase_add_test(tc_core, test_sscanf_int_positive_and_zeros);
  tcase_add_test(tc_core, test_sscanf_int_zero);
  tcase_add_test(tc_core, test_sscanf_int_negative_zero);
  tcase_add_test(tc_core, test_sscanf_int_positive_zero);
  tcase_add_test(tc_core, test_sscanf_int_many_int);
  tcase_add_test(tc_core, test_sscanf_int_width1);
  tcase_add_test(tc_core, test_sscanf_int_width2);
  tcase_add_test(tc_core, test_sscanf_int_width3);
  tcase_add_test(tc_core, test_sscanf_int_width4);
  tcase_add_test(tc_core, test_sscanf_int_width1);
  tcase_add_test(tc_core, test_sscanf_char);
  //tcase_add_test(tc_core, test_sscanf_char_with_spaces);
  tcase_add_test(tc_core, test_sscanf_i_dec);
  tcase_add_test(tc_core, test_sscanf_i_octal);
  //tcase_add_test(tc_core, test_sscanf_i_octal_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_i_octal_uncorrect);
  tcase_add_test(tc_core, test_sscanf_i_hex);
  tcase_add_test(tc_core, test_sscanf_i_hex_uncorrect);
  //tcase_add_test(tc_core, test_sscanf_i_hex_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_e_float);
  //tcase_add_test(tc_core, test_sscanf_e_float_neg_pos_width_delim);
  tcase_add_test(tc_core, test_sscanf_e_double);
  tcase_add_test(tc_core, test_sscanf_e_long_double);
  tcase_add_test(tc_core, test_sscanf_E_f_g_G);
  tcase_add_test(tc_core, test_sscanf_octal);

  suite_add_tcase(s, tc_core);
  return s;
}

