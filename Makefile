CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lm -lsubunit

SRC = s21_memchr.c
OBJ = $(SRC:.c=.o)
NAME = s21_string.a

TEST_SRC = tests/test_main.c tests/test_memchr.c
TEST_BIN = test_app

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(NAME)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -I. tests/test_main.c tests/test_memchr.c $(NAME) -o $(TEST_BIN) $(CHECK_LIBS) -lm -lsubunit
	./$(TEST_BIN)

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