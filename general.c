#include "question.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <sys/select.h>

bool compare_exit( char* buf ){
	char Exit_string[10] = "exit";
	int length = strlen(buf);
	if( length != 4 )return false;
	for(int x=0;x<4;x++){
		buf[x] = tolower( buf[x] );
	}
	if( strcmp( Exit_string , buf ) == 0 ) return true;
	return false;
}

bool get_reply(){
	char buf[100] = {};
	read( STDIN_FILENO , buf , sizeof(buf) );
	if( buf[0] == 'y' || buf[0] == 'Y' ) return true;
	return false;
}

char get_choose(){
	char buf[100] = {};
	while( true ){
		bool pass = true;
		memset( buf , '\0' , 100 );
		int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		if( compare_exit(buf) ){
			fprintf( stderr , "[Event] Bye bye~~~\n");
			exit(0);
		}
		if( bytes != 2 ){
			fprintf( stderr , "[Event] Only key a letter is enough.\n");
			pass = false;
		}
		if( buf[0] < 'A' || buf[0] > 'D' ){
			fprintf( stderr , "[Event] only ( A / B / C / D )\n");
			pass = false;
		}
		if( pass ) break;
		fprintf( stderr , "[Event] Please enter again.\n");
		fprintf( stderr , "[Event] Your answer:");
	}
	return buf[0];
}

bool random_question( int* attribute ){
	char buf[200] = {};
	char correct_answer = 'X';
	int fd = open( "./log/question.txt" , O_RDONLY );
	int num_question = -1;
	int bacteria = rand()%4;
	int question_position = rand() % num_question;
	Question question;
	*attribute = bacteria;

	if     ( bacteria == 0 ) fprintf( stderr , "[Event] Bumping into a Smallpox, your HP will be taken because it cause you ill if WA.\n");
	else if( bacteria == 1 ) fprintf( stderr , "[Event] Bumping into a HIV, your Defense will be weaker because your immunity system is affected if WA.\n");
	else if( bacteria == 2 ) fprintf( stderr , "[Event] Bumping into a Neurotropic, your ATK will be less because of its nerve damage if WA.\n");
	else if( bacteria == 3 ) fprintf( stderr , "[Event] Bumping into a COVID-19, your money will disappear because you buying many many mask if WA.\n");
	sleep(2);

	memset( buf , '\0' , 200 );
	lseek( fd , 100*sizeof(Question) , SEEK_SET );
	read( fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &num_question );
	
	lseek( fd , question_position*sizeof(Question) , SEEK_SET );
	read( fd , &question , sizeof(Question) );
	
	// random sort question option
	bool appeared[question.type];
	for(int x=0;x<question.type;x++){
		appeared[x] = false;
	}
	fprintf( stderr , "[Description] %s\n", question.description );
	fprintf( stderr , "[Option] ");
	for(int x=0;x<question.type;x++){
		char c = 'A' + x;
		int should_appear = -1;
		while( true ){
			should_appear = rand()%question.type;
			if( !appeared[should_appear] ) break;
		}
		if( should_appear == 0 ) correct_answer = 'A' + x;
		fprintf( stderr , "(%c) %s ", c , question.option[should_appear] );
		appeared[should_appear] = true;
	}
	sleep(1);
	fprintf( stderr , "\n");
	fprintf( stderr , "[Event] Your answer:");
	
	char self_answer = get_choose();
	fprintf( stderr , "\n");
	if( self_answer != correct_answer ){
		return false;
	}
	return true;
}

void pass_away( char* character , int ID , char* name ){
	char file_name[30] = {};
	char buf[100] = {};
	sprintf( file_name , "./log/%s_info.txt" , character );
	int fd = open( file_name , O_RDWR );
	int num_account = -1;
	perror("open_log");
	
	lseek( fd , 10*sizeof(Status) , SEEK_SET );
	memset( buf , '\0' , 100 );
	read( fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &num_account );
	num_account --;
	memset( buf , '\0' , 100 );
	sprintf( buf , "%d" , num_account );
	lseek( fd , 10*sizeof(Status) , SEEK_SET );
	write( fd , buf , strlen(buf) );
	
	Status temp;
	for(int x=ID+1;x<=num_account;x++){
		lseek( fd , x*sizeof(Status) , SEEK_SET );
		read( fd , &temp , sizeof(Status) );
		lseek( fd , (-2)*sizeof(Status) , SEEK_CUR );
		write( fd , &temp , sizeof(Status) );
	}


	fprintf( stderr , "[System] QQQQ dear %s you leave us a cherish memory QAQ.\n", name );
	sleep(2);
	fprintf( stderr , "[System] but.....\n");
	sleep(2);
	fprintf( stderr , "[System]         I will..\n");
	sleep(2);

	struct timeval tempo;
	tempo.tv_sec = 0;
	tempo.tv_usec = 500;
	select( 0 , 0 , 0 , 0 , &tempo );
}
