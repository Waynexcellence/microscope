#include "general.c"

int main(){
	fprintf( stderr , "To play, you can type \e[7m./gamestart\e[0m\n" );
	fprintf( stderr , "To add a new question, you can type \e[7m./push_question\e[0m\n" );
	fprintf( stderr , "To delete a character/question, you can type \e[7m./delete\e[0m\n" );
	fprintf( stderr , "To reset all data, you can type \e[7m./reset\e[0m\n" );
	return 0;
}
