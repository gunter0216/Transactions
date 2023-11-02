CC=g++
STD=-std=c++17
WWW=-Wall -Wextra -Werror
MODEL=hashtable/hash_table.cc tree/treemainfoo.cc tree/tree.cc
TESTFLAGS= -lgtest -pthread -lstdc++ -lgtest_main
VIEW=view/console_interface.cc view/console_style.cc

all: test start
	$(GA)

hash_table.a:
	@$(CC) $(STD) $(WWW) -c hashtable/hash_table.cc
	@ar rcs hash_table.a hash_table.o
	@rm *.o

self_balancing_binary_search_tree.a:
	@$(CC) $(STD) $(WWW) -c tree/tree.cc tree/treemainfoo.cc
	@ar rcs self_balancing_binary_search_tree.a treemainfoo.o tree.o
	@rm *.o

tests: test

test: clean
	@$(CC) $(STD) $(WWW) $(MODEL) ./tests/tests.cc $(TESTFLAGS) -o test
	./test

lcov: clean
	@$(CC) $(STD) $(WWW) $(MODEL) ./tests/tests.cc $(TESTFLAGS) -fprofile-arcs -ftest-coverage -o test
	@./test
	@lcov --no-external -c -d . -o tests.info
	@genhtml tests.info -o report
	@open report/index.html
	@rm -rf *.gcda *.gcno *.info

build: clean hash_table.a self_balancing_binary_search_tree.a
	@$(CC) $(STD) $(WWW) $(VIEW) hash_table.a self_balancing_binary_search_tree.a main.cc -o Transactions

start: build
	./Transactions

cppcheck:
	cppcheck --enable=all --suppress=missingInclude --suppress=unusedFunction --std=c++17 --language=c++  ./*/*.cc ./*/*.h

lint:
	@clang-format --style=Google -i ./*/*h ./*/*cc
	@clang-format --style=Google -n ./*/*h ./*/*cc

clean:
	@rm -rf *.a *.o *.gcda *.gcno *.info *.out report Transactions test

