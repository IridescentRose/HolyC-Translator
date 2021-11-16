all:
	gcc -Wall -Wextra -Werror -std=c99 -pedantic -O2 *.c -o hc2c

clean:
	rm hc2c

valgrind:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./hc2c tests/test1.hc output.c