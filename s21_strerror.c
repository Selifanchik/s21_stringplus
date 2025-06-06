#include "s21_strerror.h"

#include "s21_string.h"

char *s21_strerror(int errnum) {
  const char *errors_array[ERR_N] = ERROR_LIST;
  static char result[60];

  if (errnum >= 0 && errnum < ERR_N) {
    s21_sprintf(result, "%s", errors_array[errnum]);
  } else {
#ifdef __linux__
    s21_sprintf(result, "Unknown error %d", errnum);
#elif __APPLE__
    s21_sprintf(result, "Unknown error: %d", errnum);
#endif
  }

  return result;
}