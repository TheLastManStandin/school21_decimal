CC = gcc

CFLAGS = -std=c11 -Wall -Wextra -Werror
PATH_CHECK = -I/opt/homebrew/include -L/opt/homebrew/lib
LIBS_TEST = -lcheck -lm -lpthread

ifeq ($(shell uname), Linux)
	LIBS_TEST += -lsubunit
endif

SRC = s21_decimal_arithmetic.c \
      s21_decimal_compare.c \
      s21_decimal_convert.c \
      s21_decimal_other.c \
      s21_decimal_utils.c

TEST_SRCS = tests/*.c
TEST_BIN = test_all

GCOV_BIN = gcov_all
REPORT_DIR = report
COV_FLAGS = --coverage

LIB_NAME = s21_decimal.a

.PHONY: all clean rebuild test gcov_report clangformat

all: $(LIB_NAME)

$(LIB_NAME):
	$(CC) $(CFLAGS) -c $(SRC)
	ar rcs $(LIB_NAME) *.o
	rm -f *.o

test: clean $(LIB_NAME)
	$(CC) $(PATH_CHECK) $(CFLAGS) $(TEST_SRCS) $(LIB_NAME) $(LIBS_TEST) -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: clean
	$(CC) $(PATH_CHECK) $(CFLAGS) $(COV_FLAGS) $(SRC) $(TEST_SRCS) $(LIBS_TEST) -o $(GCOV_BIN)
	./$(GCOV_BIN)
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '*/tests/*' --output-file coverage_filtered.info
	genhtml coverage_filtered.info --output-directory $(REPORT_DIR)

clean:
	rm -rf *.o *.a *.gcno *.gcda *.gcov *.info \
		coverage.info coverage_filtered.info \
		$(TEST_BIN) $(GCOV_BIN) $(REPORT_DIR)

rebuild: clean all

clangformat:
	clang-format -n *.c *.h tests/*.c tests/*.h -i