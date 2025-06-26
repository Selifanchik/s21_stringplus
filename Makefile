# ===== Компилятор и флаги =====
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
OS_NANE := $(shell uname -s)
CHECK_LIBS = -lcheck -lm
ifeq ($(OS_NANE), Darwin)
	OPEN_GCOV = open
endif
ifeq ($(OS_NANE), Linux)
	OPEN_GCOV = xdg-open
	CHECK_LIBS += -lsubunit
endif

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

# ===== Цель: полная пересборка без покрытия =====
re: fclean s21_string.a test_build

# ===== Цель: сборка библиотеки без покрытия =====
s21_string.a: $(OBJ)
	ar rcs $(LIB_NAME) $(OBJ)
	ranlib $(LIB_NAME)

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
gcov_report: re coverage_build
	./$(TEST_BIN)
	lcov --capture --directory . --output-file coverage.info --rc branch_coverage=1
	lcov --remove coverage.info '*/$(TEST_DIR)/*' --output-file coverage_filtered.info --rc branch_coverage=1
	genhtml coverage_filtered.info --output-directory coverage --rc branch_coverage=1
	$(OPEN_GCOV) ./coverage/src/index.html &

# ===== Сборка тестового приложения =====
$(TEST_BIN): $(TEST_OBJ) s21_string.a
	$(CC) $(CFLAGS) -I. $(TEST_OBJ) $(LIB_NAME) -o $(TEST_BIN) $(CHECK_LIBS)

cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 --quiet --force \
  --suppress=missingIncludeSystem src/ $(TEST_DIR)/

# ===== Цель: проверка на утечки памяти с помощью valgrind =====
valgrind: test_build
	valgrind --tool=memcheck --leak-check=yes ./$(TEST_BIN)

%_suite: test_build
	CK_RUN_SUITE="$*" valgrind --tool=memcheck --leak-check=yes ./$(TEST_BIN)

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
	rm -f *.info $(TEST_BIN)
	rm -rf coverage

fclean: clean
	rm -f $(LIB_NAME)

format:
	clang-format -i *.c *.h tests/*.c