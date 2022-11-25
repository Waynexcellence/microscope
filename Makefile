all:
	gcc -std=c99 -Wall gamestart.c -o gamestart
	gcc -std=c99 -Wall bacteria.c -o bacteria
	gcc -std=c99 -Wall human.c -o human
	gcc -std=c99 -Wall reset.c -o reset
	gcc -std=c99 -Wall push_question.c -o push_question
clean:
	rm -f gamestart bacteria human reset push_question
