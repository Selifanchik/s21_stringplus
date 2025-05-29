CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lm -lsubunit

SRC = s21_memchr.c s21_memcmp.c s21_memcpy.c s21_memset.c s21_strncat.c s21_strchr.c s21_strncmp.c s21_strncpy.c s21_strcspn.c s21_strerror.c s21_strlen.c s21_strpbrk.c s21_strrchr.c s21_strstr.c
OBJ = $(SRC:.c=.o)
NAME = s21_string.a

TEST_SRC = $(wildcard tests/*.c)
TEST_BIN = test_app

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(NAME)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -I. $(TEST_SRC) $(NAME) -o $(TEST_BIN) $(CHECK_LIBS)
	./$(TEST_BIN)

valgrind: $(NAME)
	$(CC) $(CFLAGS) -I. $(TEST_SRC) $(NAME) -o $(TEST_BIN) $(CHECK_LIBS)
	valgrind --leak-check=full --show-leak-kinds=definite --track-origins=yes ./$(TEST_BIN)

valgrind_suite:
	CK_RUN_SUITE=s21_strncmp valgrind --leak-check=full --show-leak-kinds=definite --track-origins=yes ./$(TEST_BIN)

gcov_report: test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	@echo "HTML report generated in ./coverage/index.html"

clean:
	rm -f *.o *.gcno *.gcda *.info $(TEST_BIN) coverage.info
	rm -rf coverage

fclean: clean
	rm -f $(NAME)

re: fclean all