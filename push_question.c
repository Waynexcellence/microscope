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
#include "question.h"

int main(){
	Question question;
	question.correct_answer = 0;
	char buf[200] = {};
	int bytes = -1;
	int num_question = -1;
	int fd = open("./log/question.txt" , O_RDWR );
	perror("open question.txt");
	lseek( fd , 100*sizeof(Question) , SEEK_SET );
	memset( buf , '\0' , 200 );
	bytes = read( fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &num_question );

	if( num_question >= 95 ){
		fprintf( stderr , "Too many question.\n");
		return 0;
	}
	
	fprintf( stderr , "[System] You have been enrolled %d question.\n", num_question );
	fprintf( stderr , "[System] Please enter the question type.\n");
	fprintf( stderr , "[System] 2 for ( True/False )\n");
	fprintf( stderr , "[System] 4 for  (  Select  )\n");
	fprintf( stderr , "[System] Your question type:");
	memset( buf , '\0' , 200 );
	bytes = read( STDIN_FILENO , buf , sizeof(buf) );
	sscanf( buf , "%d" , &question.type );

	fprintf( stderr , "[System] Please enter your question description.\n");
	fprintf( stderr , "[System] Your question description:");
	memset( buf , '\0' , 200 );
	bytes = read( STDIN_FILENO , buf , sizeof(buf) );
	buf[bytes-1] = 0;
	strcpy( question.description , buf );
	
	for(int x=0;x<question.type;x++){
		char c = 'A' + x;
		fprintf( stderr , "[System] Your question option %c is:", c );
		memset( buf , '\0' , 200 );
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		strcpy( question.option[x] , buf );
	}
	
	lseek( fd , num_question*sizeof(Question) , SEEK_SET );
	write( fd , &question , sizeof(Question) );

	num_question ++;
	memset( buf , '\0' , 200 );
	sprintf( buf , "%d" , num_question );

	lseek( fd , 100*sizeof(Question) , SEEK_SET );
	write( fd , buf , strlen(buf) );
	
	return 0;
}
