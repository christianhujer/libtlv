CPPFLAGS:=-I../include
CFLAGS:=-MMD --std=c99 -W -Wall -pedantic --coverage
ARFLAGS:=-crs

.PHONY: all
.PRECIOUS: %.o
all: libtlv.a(tlv.o)

.PHONY: clean
clean::
	$(RM) *.[adios] *.gcda *.gcno

include $(wildcard *.d)
