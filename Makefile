# Компилятор и флаги
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lm -lsubunit

# Включение флагов покрытия при необходимости
GCOV ?= 0
ifeq ($(GCOV), 1)
    CFLAGS += $(GCOV_FLAGS)
endif

# Файлы библиотеки
SRC = s21_memchr.c s21_memcmp.c s21_memcpy.c s21_memset.c \
      s21_strncat.c s21_strchr.c s21_strncmp.c s21_strncpy.c \
      s21_strcspn.c s21_strerror.c s21_strlen.c s21_strpbrk.c \
      s21_strrchr.c s21_strstr.c s21_strtok.c

OBJ = $(SRC:.c=.o)
NAME = s21_string.a

# Тестовые файлы
TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_BIN = test_app

# ===== Цель: all (полная сборка, запуск и отчёт покрытия) =====
all: re gcov_report

# ===== Цель: полная пересборка без покрытия =====
re: fclean s21_string.a test_build

# ===== Цель: сборка библиотеки без покрытия =====
s21_string.a: $(OBJ)
	ar rcs $(NAME) $(OBJ)

# ===== Цель: сборка тестового приложения без покрытия =====
test_build: $(TEST_BIN)

# ===== Цель: запуск тестов (предполагается, что собраны) =====
test: test_build
	./$(TEST_BIN)

# ===== Цель: сборка с покрытием (пересборка библиотеки и тестов с флагами покрытия) =====
coverage_build:
	$(MAKE) clean
	$(MAKE) GCOV=1 s21_string.a
	$(MAKE) GCOV=1 test_build

# ===== Цель: генерация отчёта покрытия, зависит от сборки с покрытием и запуска тестов =====
gcov_report: coverage_build
	./$(TEST_BIN)
	@echo "Capturing coverage data for library source files..."
	lcov --capture --directory . --output-file coverage.info --rc branch_coverage=1
	@echo "Removing test sources from coverage data..."
	lcov --remove coverage.info '$(TEST_DIR)/*' --output-file coverage_filtered.info --rc branch_coverage=1
	@echo "Generating HTML report..."
	genhtml coverage_filtered.info --output-directory coverage --rc branch_coverage=1
	@echo "HTML report generated in ./coverage/index.html"

# ===== Сборка тестового приложения =====
$(TEST_BIN): $(TEST_OBJ) s21_string.a
	$(CC) $(CFLAGS) -I. $(TEST_OBJ) $(NAME) -o $(TEST_BIN) $(CHECK_LIBS)

# Компиляция объектных файлов библиотеки
%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция объектных файлов тестов
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c s21_string.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

# ===== Очистка =====
clean:
	find . \( -name "*.o" -o -name "*.gcno" -o -name "*.gcda" \) -delete
	rm -f *.info $(TEST_BIN)
	rm -rf coverage

fclean: clean
	rm -f $(NAME)