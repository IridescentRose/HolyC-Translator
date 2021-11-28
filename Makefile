all:
	@mkdir -p bin
	@mkdir -p out
	@$(CC) -Wall -Wextra -Werror -std=gnu99 -pedantic -g -O2 src/*.c src/tokenizer/*.c src/parser/*.c -o bin/hc2c

clean:
	@rm -rf bin
	@rm -rf out

test: all
	@$(CC) -Wall -Wextra -Werror -std=gnu99 -pedantic -g -O2 tests/tester.c -o bin/tester && bin/tester


valgrind:
	@valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes bin/hc2c tests/test25.hc out/output.c