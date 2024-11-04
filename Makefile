# -fsanitize=... documentation:
#   https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Instrumentation-Options.html
#   https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer

# CFLAGS=-Wall -Werror -g -fsanitize=address
CFLAGS=-Wall -Werror -g 
TARGETS=clist_test
SRC_DIR=src  # Define source directory

all: $(TARGETS)

$(TARGETS): ./src/clist.o clist_test.o
	gcc $(CFLAGS) ./src/clist.o clist_test.o -o $(TARGETS)

./src/clist.o: ./src/clist.c ./src/clist.h
	gcc $(CFLAGS) -c ./src/clist.c -o ./src/clist.o

clist_test.o: clist_test.c ./src/clist.h
	gcc $(CFLAGS) -c clist_test.c -o clist_test.o

clean:
	rm -f $(TARGETS) ./src/*.o *.o
