all:
	gcc -std=c99 -Wall gamestart.c -o gamestart
	gcc -std=c99 -Wall bacteria.c -o bacteria
	gcc -std=c99 -Wall human.c -o human
	gcc -std=c99 -Wall reset.c -o reset
	gcc -std=c99 -Wall push_question.c -o push_question
	gcc -std=c99 -Wall delete.c -o delete
	gcc -std=c99 -Wall help.c -o help
cp:
	cp gamestart.c ./store/
	cp bacteria.c ./store/
	cp human.c ./store/
	cp reset.c ./store/
	cp push_question.c ./store/
	cp delete.c ./store/
	cp general.c ./store
	cp help.c ./store
clean:
	rm -f gamestart bacteria human reset push_question delete help
	rm -f *~

