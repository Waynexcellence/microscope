all:
	gcc -std=c99 -Wall gamestart.c -o gamestart
	gcc -std=c99 -Wall bacteria.c -o bacteria
	gcc -std=c99 -Wall human.c -o human
clean:
	rm -f gamestart bacteria human
