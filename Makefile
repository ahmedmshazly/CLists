# -fsanitize=... documentation:
#   https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Instrumentation-Options.html
#   https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer

CFLAGS=-Wall -Werror -g -fsanitize=address
# CFLAGS=-Wall -Werror -g 
TARGETS=clist_test

all: $(TARGETS)

$(TARGETS): ./clist.o clist_test.o
	gcc $(CFLAGS) ./clist.o clist_test.o -o $(TARGETS)

./clist.o: ./clist.c ./clist.h
	gcc $(CFLAGS) -c ./clist.c -o ./clist.o

clist_test.o: clist_test.c ./clist.h
	gcc $(CFLAGS) -c clist_test.c -o clist_test.o

clean:
	rm -f $(TARGETS) ./*.o *.o
