#include <check.h>
#include <stdio.h>
#include <string.h>
// #include <stdint.h>

#include "s21_string.h"

// Базовый случай: строки одинаковые
START_TEST(test_memcmp_equal) {
  char str1[] = "hello world";
  char str2[] = "hello world";
  int my_cmp = s21_memcmp(str1, str2, 12);
  int original_cmp = memcmp(str1, str2, 12);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Отличающися байт больше в строке1
START_TEST(test_memcmp_first_more) {
  char str1[] = "abc";
  char str2[] = "aba";

  int my_cmp = s21_memcmp(str1, str2, 4);
  int original_cmp = memcmp(str1, str2, 4);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Отличающися байт больше в строке2
START_TEST(test_memcmp_first_less) {
  char str1[] = "aba";
  char str2[] = "abc";
  int my_cmp = s21_memcmp(str1, str2, 4);
  int original_cmp = memcmp(str1, str2, 4);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Отличается первый байт
START_TEST(test_memcmp_diff_first_byte) {
  char str1[] = "abc";
  char str2[] = "xbc";
  int my_cmp = s21_memcmp(str1, str2, 3);
  int original_cmp = memcmp(str1, str2, 3);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Частично отличаются, но проверка не доходит до отличия
START_TEST(test_memcmp_partial_equal) {
  char str1[] = "abcdef";
  char str2[] = "abcxyz";
  int my_cmp = s21_memcmp(str1, str2, 3);
  int original_cmp = memcmp(str1, str2, 3);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Длина равна нулю
START_TEST(test_memcmp_zero_length) {
  char str1[] = "abcdef";
  char str2[] = "abcxyz";
  int my_cmp = s21_memcmp(str1, str2, 0);
  int original_cmp = memcmp(str1, str2, 0);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// С нуль символом в середине
START_TEST(test_memcmp_with_null_bytes) {
  char str1[] = {'a', '\0', 'c'};
  char str2[] = {'a', '\0', 'd'};
  int my_cmp = s21_memcmp(str1, str2, 3);
  int original_cmp = memcmp(str1, str2, 3);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

// Массив байтов с нулем, не строка
START_TEST(test_memcmp_with_unsigned_char) {
  unsigned char str1[] = {0xFF, 0x00, 0x7F};
  unsigned char str2[] = {0xFE, 0x00, 0x7F};
  int my_cmp = s21_memcmp(str1, str2, 3);
  int original_cmp = memcmp(str1, str2, 3);

  if (my_cmp < 0)
    ck_assert_int_lt(original_cmp, 0);
  else if (my_cmp > 0)
    ck_assert_int_gt(original_cmp, 0);
  else
    ck_assert_int_eq(original_cmp, 0);
}
END_TEST

Suite *memcmp_suite(void) {
  Suite *s = suite_create("s21_memcmp");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_memcmp_equal);
  tcase_add_test(tc, test_memcmp_first_more);
  tcase_add_test(tc, test_memcmp_first_less);
  tcase_add_test(tc, test_memcmp_diff_first_byte);
  tcase_add_test(tc, test_memcmp_partial_equal);
  tcase_add_test(tc, test_memcmp_zero_length);
  tcase_add_test(tc, test_memcmp_with_null_bytes);
  tcase_add_test(tc, test_memcmp_with_unsigned_char);

  suite_add_tcase(s, tc);
  return s;
}
