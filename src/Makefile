CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
    CHECK=-lcheck
else
    CHECK=-lcheck -lm -lsubunit -lrt -lpthread
endif

all: clean p c test gcov_report


s21_decimal.o: clean
	@$(CC) $(CFLAGS) -c s21_decimal.c -o s21_decimal.o

clean:
	@rm -rf *.g* report.* *.out* *.o ./report s21_decimal.a *.txt *.cfg

gcov_report: clean
	$(CC) $(CFLAGS) --coverage -c s21_decimal.c -o s21_decimal.o
	$(CC) $(CFLAGS) --coverage test.c s21_decimal.o $(CHECK)
	./a.out
	@gcov -b -l -p -c s21_decimal.gcno; gcovr -g -k -r . --html --html-details -o report.html

test: s21_decimal.o
	@$(CC) $(CFLAGS) test.c s21_decimal.o $(CHECK)
	@./a.out

p: clean
	@cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	@python3 ../materials/linters/cpplint.py --extensions=c ./*
	
c: clean
	@cppcheck --enable=all --suppress=missingIncludeSystem ./

l: clean
	$(CC) $(CFLAGS) -g -c s21_decimal.c -o s21_decimal.o
	$(CC) $(CFLAGS) -g test.c s21_decimal.o $(CHECK)
	valgrind -q -s --leak-check=full --trace-children=yes --track-origins=yes --log-file=RESULT_VALGRIND.txt ./a.out
	cat RESULT_VALGRIND.txt

s21_decimal.a: clean
	$(CC) $(CFLAGS) -c s21_decimal.c -o s21_decimal.o
	ar rc s21_decimal.a s21_decimal.o
	ranlib s21_decimal.a
