All:
	gcc -c file.c -o file.o
	gcc file.o -o file
clean:
	rm -f file.o
	rm -f file
