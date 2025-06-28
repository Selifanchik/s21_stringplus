#include <check.h>
#include <string.h>

#include "../s21_string.h"

// Базовый случай
START_TEST(test_strtok_basic) {
  char str1[] = "Hello world";
  char str2[] = "Hello world";
  const char *delim = " ";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// Подряд идущие делители, в начале, в конце
START_TEST(test_strtok_multiple_delim) {
  char str1[] = "  Hello  world  I'm from s21  ";
  char str2[] = "  Hello  world  I'm from s21  ";
  const char *delim = " ";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// В строке только делители
START_TEST(test_strtok_only_delims) {
  char str1[] = "!!!!!!!!!!!";
  char str2[] = "!!!!!!!!!!!";
  const char *delim = "!";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);  // NULL
}
END_TEST

// Делителя нет в строке
START_TEST(test_strtok_delim_no_match) {
  char str1[] = "Hello world";
  char str2[] = "Hello world";
  const char *delim = "!";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);  // вся строка
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// Несколько делителей
START_TEST(test_strtok_some_delims) {
  char str1[] = "Hello world";
  char str2[] = "Hello world";
  const char *delim = "ol";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// Табы и пробелы делители
START_TEST(test_strtok_spaces_and_tabs_delims) {
  char str1[] = "  Hello\t  world  I'm from\t s21  ";
  char str2[] = "  Hello\t  world  I'm from\t s21  ";
  const char *delim = "\t ";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// Делитель ноль
START_TEST(test_strtok_null_delim) {
  char str1[] = "Hello world";
  char str2[] = "Hello world";
  const char *delim = "\0";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);  // вся строка
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// Пустая строка
START_TEST(test_strtok_empty_str) {
  char str1[] = "";
  char str2[] = "";
  const char *delim = "s";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != S21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);  // NULL
}
END_TEST

START_TEST(test_strtok_multiple_strings) {
  char str1[] = "one,two";
  char str2[] = "one,two";
  const char *delim = ",";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);
  ck_assert_str_eq(token1, token2);

  token1 = s21_strtok(S21_NULL, delim);
  token2 = strtok(NULL, delim);
  ck_assert_str_eq(token1, token2);

  // Запускаем разбор новой строки
  char str3[] = "three,four";
  char str4[] = "three,four";

  token1 = s21_strtok(str3, delim);
  token2 = strtok(str4, delim);
  ck_assert_str_eq(token1, token2);
}
END_TEST

// Разный делитель
START_TEST(test_strtok_diff_delim) {
  char str1[] = "ddffggamkobbab";
  char str2[] = "ddffggamkobbab";
  const char *delim = "a";
  const char *delim2 = "b";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);
  ck_assert_str_eq(token1, token2);

  token1 = s21_strtok(S21_NULL, delim2);
  token2 = strtok(NULL, delim2);
  ck_assert_str_eq(token1, token2);
}
END_TEST

Suite *strtok_suite(void) {
  Suite *s = suite_create("s21_strtok");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strtok_basic);
  tcase_add_test(tc_core, test_strtok_multiple_delim);
  tcase_add_test(tc_core, test_strtok_only_delims);
  tcase_add_test(tc_core, test_strtok_delim_no_match);
  tcase_add_test(tc_core, test_strtok_some_delims);
  tcase_add_test(tc_core, test_strtok_spaces_and_tabs_delims);
  tcase_add_test(tc_core, test_strtok_null_delim);
  tcase_add_test(tc_core, test_strtok_empty_str);
  tcase_add_test(tc_core, test_strtok_multiple_strings);
  tcase_add_test(tc_core, test_strtok_diff_delim);

  suite_add_tcase(s, tc_core);
  return s;
}
