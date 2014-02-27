all: test
debug:
	gcc -g -o test test.c ../src/cdg.c ../src/stack.c
	gdb ./test
	rm ./test
test:
	gcc -o test test.c ../src/cdg.c ../src/stack.c
	./test
	rm ./test



