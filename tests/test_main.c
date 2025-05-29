#include <check.h>
#include <stdlib.h>

Suite *memchr_suite(void);
Suite *memcmp_suite(void);
Suite *memcpy_suite(void);
Suite *memset_suite(void);
Suite *strncat_suite(void);
Suite *strchr_suite(void);
Suite *strncmp_suite(void);
Suite *strncpy_suite(void);
Suite *strcspn_suite(void);
Suite *strerror_suite(void);
Suite *strlen_suite(void);
Suite *strpbrk_suite(void);
Suite *strrchr_suite(void);
Suite *strstr_suite(void);

int main(void) {
  int number_failed = 0;
  SRunner *sr = srunner_create(memchr_suite());

  srunner_add_suite(sr, memcmp_suite());
  srunner_add_suite(sr, memcpy_suite());
  srunner_add_suite(sr, memset_suite());
  srunner_add_suite(sr, strncat_suite());
  srunner_add_suite(sr, strchr_suite());
  srunner_add_suite(sr, strncmp_suite());
  srunner_add_suite(sr, strncpy_suite());
  srunner_add_suite(sr, strcspn_suite());
  srunner_add_suite(sr, strerror_suite());
  srunner_add_suite(sr, strlen_suite());
  srunner_add_suite(sr, strpbrk_suite());
  srunner_add_suite(sr, strrchr_suite());
  srunner_add_suite(sr, strstr_suite());

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
