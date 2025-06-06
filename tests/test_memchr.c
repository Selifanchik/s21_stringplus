#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай: символ найден
START_TEST(test_memchr_found) {
  char str[] = "hello world";
  ck_assert_ptr_eq(s21_memchr(str, 'o', 11), memchr(str, 'o', 11));
}
END_TEST

// Символ не найден
START_TEST(test_memchr_not_found) {
  char str[] = "abcdef";
  ck_assert_ptr_eq(s21_memchr(str, 'x', 6), memchr(str, 'x', 6));
}
END_TEST

// Ищем нулевой символ ('\0')
START_TEST(test_memchr_null_char) {
  char str[] = "abc\0def";
  ck_assert_ptr_eq(s21_memchr(str, '\0', 7), memchr(str, '\0', 7));
}
END_TEST

// Длина поиска = 0
START_TEST(test_memchr_zero_length) {
  char str[] = "abcdef";
  ck_assert_ptr_eq(s21_memchr(str, 'a', 0), memchr(str, 'a', 0));
}
END_TEST

// Символ находится в последнем байте
START_TEST(test_memchr_last_byte) {
  char *str = "12345";
  ck_assert_ptr_eq(s21_memchr(str, '5', 5), memchr(str, '5', 5));
}
END_TEST

// Строка полностью из одинаковых символов
START_TEST(test_memchr_repeated_chars) {
  char str[] = "aaaaa";
  ck_assert_ptr_eq(s21_memchr(str, 'a', 5), memchr(str, 'a', 5));
}
END_TEST

// Поиск по пустой строке
START_TEST(test_memchr_empty_string) {
  char str[] = "";
  ck_assert_ptr_eq(s21_memchr(str, 'a', 1), memchr(str, 'a', 1));
}
END_TEST

// Поиск непечатных символов
START_TEST(test_memchr_nonprinting_chars) {
  char str[] = {0x01, 0x02};
  ck_assert_ptr_eq(s21_memchr(str, 2, 2), memchr(str, 2, 2));
}
END_TEST

// Поиск нуля по пустой строке
START_TEST(test_memchr_found_null_empty_string) {
  char str[] = "";
  ck_assert_ptr_eq(s21_memchr(str, '\0', 1), memchr(str, '\0', 1));
}
END_TEST

// Поиск пересекающиеся области
START_TEST(test_memchr_found_overlaps) {
  char str[] = "abcd";
  ck_assert_ptr_eq(s21_memchr(str, *(str+1), 3), memchr(str, *(str+1), 3));
}
END_TEST

Suite *memchr_suite(void) {
  Suite *s = suite_create("s21_memchr");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_memchr_found);
  tcase_add_test(tc, test_memchr_not_found);
  tcase_add_test(tc, test_memchr_null_char);
  tcase_add_test(tc, test_memchr_zero_length);
  tcase_add_test(tc, test_memchr_last_byte);
  tcase_add_test(tc, test_memchr_repeated_chars);
  tcase_add_test(tc, test_memchr_empty_string);
  tcase_add_test(tc, test_memchr_nonprinting_chars);
  tcase_add_test(tc, test_memchr_found_null_empty_string);
  tcase_add_test(tc, test_memchr_found_overlaps);

  suite_add_tcase(s, tc);
  return s;
}
