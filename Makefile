# ===== Компилятор и флаги =====
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lm -lsubunit

# ===== Включение флагов покрытия при необходимости =====
GCOV ?= 0
ifeq ($(GCOV), 1)
    CFLAGS += $(GCOV_FLAGS)
endif

# ===== Файлы библиотеки =====
SRC = $(wildcard s21_*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
LIB_NAME = s21_string.a

# ===== Тестовые файлы =====
TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(patsubst %.c, %.o, $(TEST_SRC))
TEST_BIN = test_app

# ===== Цель: all (полная сборка, запуск и отчёт покрытия) =====
all: s21_string.a

# ===== Полная пересборка без покрытия =====
rebuild: clean $(LIB_NAME) test_build

# ===== Сборка библиотеки без покрытия =====
s21_string.a: $(OBJ)
	ar rcs $(LIB_NAME) $(OBJ)
	ranlib $(LIB_NAME)

# ===== Цель: сборка тестового приложения без покрытия =====
test_build: $(TEST_BIN)

# ===== Запуск тестов =====
test: test_build
	./$(TEST_BIN)

# ===== Сборка с покрытием (пересборка библиотеки и тестов с флагами покрытия) =====
coverage_build:
	$(MAKE) clean
	$(MAKE) GCOV=1 $(LIB_NAME)
	$(MAKE) GCOV=1 test_build

# ===== Генерация отчёта покрытия, зависит от сборки с покрытием и запуска тестов =====
gcov_report: rebuild coverage_build
	./$(TEST_BIN)
	lcov --capture --directory . --output-file coverage.info --rc branch_coverage=1
	lcov --remove coverage.info '*/$(TEST_DIR)/*' --output-file coverage_filtered.info --rc branch_coverage=1
	genhtml coverage_filtered.info --output-directory coverage --rc branch_coverage=1

# ===== Сборка тестового приложения =====
$(TEST_BIN): $(TEST_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -I. $(TEST_OBJ) $(LIB_NAME) -o $(TEST_BIN) $(CHECK_LIBS)

# ===== Проверка на cppcheck =====
cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 --quiet --force \
  --suppress=missingIncludeSystem $(SRC) $(TEST_SRC)

# ===== Проверка на утечки памяти с помощью valgrind =====
valgrind: test_build
	valgrind --tool=memcheck --leak-check=yes ./$(TEST_BIN)

# ===== Проверка на утечки памяти с помощью valgrind одной функции =====
%_suite: test_build
	CK_RUN_SUITE="$*" valgrind --tool=memcheck --leak-check=yes ./$(TEST_BIN)

# ===== Запуск тестов для одной функции =====
%_test: test_build
	CK_RUN_SUITE="$*" ./$(TEST_BIN)

# ===== Компиляция объектных файлов библиотеки =====
%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

# ===== Компиляция объектных файлов тестов =====
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c s21_string.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

# ===== Очистка =====
clean:
	find . \( -name "*.o" -o -name "*.gcno" -o -name "*.gcda" \) -delete
	rm -f *.info $(TEST_BIN) $(LIB_NAME)
	rm -rf coverage

format:
	clang-format -i -style=Google *.c *.h tests/*.c