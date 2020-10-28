CPPFLAGS:=-I../include
CFLAGS:=-MMD --std=c99 -W -Wall -pedantic -Werror --coverage
LDFLAGS:=--coverage

.PHONY: all
all: test
	valgrind -q --error-exitcode=1 --exit-on-first-error=yes ./test
	cd ../src/ && gcov -n *.o && gcovr -o /dev/null -b --fail-under-branch 100

test: ../src/libtlv.a test.o

../src/libtlv.a: ../src/*.c ../include/*.h
	$(MAKE) -C $(dir $@)

.PHONY: clean
clean::
	$(RM) test *.[adios] *.gcda *.gcno *.gcov

include $(wildcard *.d)
