all:
	@gcc -Wall -Wextra -Werror -std=gnu99 -pedantic -g -O2 *.c -o hc2c

clean:
	@rm -f hc2c

test: all
	@gcc -Wall -Wextra -Werror -std=gnu99 -pedantic -g -O2 tests/tester.c -o tester && ./tester


valgrind:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./hc2c tests/test17.hc out/output.c