CPPFLAGS:=-I../include
CFLAGS:=-MMD --std=c99 -W -Wall -pedantic -Werror --coverage
LDFLAGS:=--coverage

.PHONY: all
all: test_main
	valgrind -q --error-exitcode=1 --exit-on-first-error=yes ./test_main
	cd ../src/ && gcov -n *.o && gcovr -o /dev/null -b --fail-under-branch 100

test_main: test_main.o tlv_test.o ../src/libtlv.a

../src/libtlv.a: ../src/*.c ../include/*.h
	$(MAKE) -C $(dir $@)

.PHONY: clean
clean::
	$(RM) test_main *.[adios] *.gcda *.gcno *.gcov

include $(wildcard *.d)
