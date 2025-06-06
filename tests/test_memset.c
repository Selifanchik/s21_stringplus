#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_memset_basic) {
  char str1[10] = "abcdefghi";
  char str2[10] = "abcdefghi";

  s21_memset(str1, 'x', 4);
  memset(str2, 'x', 4);

  ck_assert_mem_eq(str1, str2, 10);
}
END_TEST

// Полное заполнение
START_TEST(test_memset_full_buffer) {
  char str1[6] = "hello";
  char str2[6] = "hello";

  s21_memset(str1, '*', 6);
  memset(str2, '*', 6);

  ck_assert_mem_eq(str1, str2, 6);
}
END_TEST

// // Заполняются 0 байтов
// START_TEST(test_memset_zero_length) {
//   char str1[6] = "world";
//   char str2[6] = "world";

//   s21_memset(str1, 0, 0);
//   memset(str2, 0, 0);

//   ck_assert_mem_eq(str1, str2, 6);
// }
// END_TEST

// Заполняются нулями
START_TEST(test_memset_null_bytes) {
  char str1[5] = {1, 2, 3, 4, 5};
  char str2[5] = {1, 2, 3, 4, 5};

  s21_memset(str1, 0, 5);
  memset(str2, 0, 5);

  ck_assert_mem_eq(str1, str2, 5);
}
END_TEST

// Заполняются пустые области
START_TEST(test_memset_empty_loc) {
  char str1[5];
  char str2[5];

  s21_memset(str1, 0, 5);
  memset(str2, 0, 5);

  ck_assert_mem_eq(str1, str2, 5);
}
END_TEST

// Спец символы
START_TEST(test_memset_spec_char) {
  char str1[5]="abcd";
  char str2[5]="abcd";

  s21_memset(str1, '\n', 4);
  memset(str2, '\n', 4);

  ck_assert_mem_eq(str1, str2, 5);
}
END_TEST

// // Проверка значения >127
// START_TEST(test_memset_unsigned_conversion) {
//   char str1[3] = "AB";
//   char str2[3] = "AB";

//   s21_memset(str1, 5555, 2);
//   memset(str2, 5555, 2);

//   ck_assert_mem_eq(str1, str2, 3);
// }
// END_TEST

Suite *memset_suite(void) {
  Suite *s = suite_create("s21_memset");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_memset_basic);
  tcase_add_test(tc, test_memset_full_buffer);
  // tcase_add_test(tc, test_memset_zero_length);
  tcase_add_test(tc, test_memset_null_bytes);
  tcase_add_test(tc, test_memset_empty_loc);
  tcase_add_test(tc, test_memset_spec_char);
  // tcase_add_test(tc, test_memset_unsigned_conversion);

  suite_add_tcase(s, tc);
  return s;
}
