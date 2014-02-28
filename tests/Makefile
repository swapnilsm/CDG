all: test
debug:
	gcc -g -o test test.c ../src/cdg.c ../src/stack.c ../src/cdgWrapper.c
	gdb ./test
	rm ./test
test:
	gcc -o test test.c ../src/cdg.c ../src/stack.c ../src/cdgWrapper.c
	./test
	rm ./test



