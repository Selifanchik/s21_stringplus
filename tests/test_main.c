#include <check.h>
#include <stdlib.h>

Suite *memchr_suite(void);
Suite *memcmp_suite(void);
Suite *memcpy_suite(void);
Suite *memset_suite(void);

int main(void) {
  int number_failed = 0;
  SRunner *sr = srunner_create(memchr_suite());

  srunner_add_suite(sr, memcmp_suite());
  srunner_add_suite(sr, memcpy_suite());
  srunner_add_suite(sr, memset_suite());

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
