#include "s21_strerror.h"

#include <stdio.h>  //УБРАТЬ

#include "s21_string.h"

char *s21_strerror(int errnum) {
  const char *errors_array[ERR_N] = ERROR_LIST;
  static char result[60];

  if (errnum >= 0 && errnum < ERR_N) {
    sprintf(result, "%s", errors_array[errnum]);
  } else {
#ifdef __linux__
    sprintf(result, "Unknown error %d", errnum);  // ЗАМЕНИТЬ
#elif __APPLE__
    sprintf(result, "Unknown error: %d", errnum);  // ЗАМЕНИТЬ
#endif
  }

  return result;
}
