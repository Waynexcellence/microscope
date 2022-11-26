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
#include "status.h"
#include "question.h"

char type_name[3][20] = { "human" , "bacteria" , "question" };
int type = -1;
int fd = -1;
int num = -1;
int delete_number = -1;

int get_type(){
	char buf[100] = {};
	int bytes = -1;
	while( true ){
		bool find = false;
		memset( buf , '\0' , 100 );
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		for(int x=0;x<bytes-1;x++){
			buf[x] = tolower(buf[x]);
		}
		for(int x=0;x<3;x++){
			if( strcmp( buf , type_name[x] ) == 0 ){
				fprintf( stderr , "\n");
				return x;
			}
		}
		if( find ) break;
		fprintf( stderr , "[System] Error please enter again.\n");
		sleep(1);
		fprintf( stderr , "[System] Your answer:\n");
	}
	return -1;
}

int open_log(){
	char file_name[30] = {};
	sprintf( file_name , "./log/%s.txt", type_name[type] );
	int temp = open( file_name , O_RDWR );
	return temp;
}

int get_num(){
	char buf[100] = {};
	int temp = -1;
	if( type == 0 || type == 1 ) lseek( fd , 10*sizeof(Status) , SEEK_SET );
	if( type == 2 ) lseek( fd , 100*sizeof(Question) , SEEK_SET );
	read( fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &temp );
	return temp;
}

void show_log(){
	Status status;
	Question question;
	lseek( fd , 0 , SEEK_SET );
	for(int x=0;x<num;x++){
		if( type == 2 ){
			read( fd , &question , sizeof(Question) );
			fprintf( stderr , " #%d [Description] %s\n", x , question.description );
		}
		else if( type == 0 || type == 1 ){
			read( fd , &status , sizeof(Status) );
			fprintf( stderr , " #%d [Name] %s\n", x , status.name );
		}
		else	fprintf( stderr , "[System] Bug, Please notify the productor.\n");
	}
	fprintf( stderr , "\n");
}

int get_delete_number(){
	char buf[100] = {};
	int bytes = -1;
	int temp = -1;
	while( true ){
		memset( buf , '\0' , 100 );
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		if( bytes < 0 ){
			fprintf( stderr , "[System] Fatal Error. Please notify the productor.\n");
			exit(0);
		}
		fprintf( stderr , "\n");
		sscanf( buf , "%d" , &temp );
		if( temp >= num || type < 0 ){
			fprintf( stderr , "[System] Please check the number you enter is in range.\n");
			fprintf( stderr , "[System] Your answer:");
			continue;
		}
		break;
	}
	return temp;
}

void modify_num(){
	char buf[100] = {};
	num --;
	sprintf( buf , "%d" , num );
	if( type == 2 ) lseek( fd , 100*sizeof(Question) , SEEK_SET );
	else if( type == 0 || type == 1 ) lseek( fd , 10*sizeof(Status) , SEEK_SET );
	else fprintf( stderr , "[System] Bug. Please notify the productor.\n");
	write( fd , buf , strlen(buf) );
}

void delete_question(){
	Question question;
	for(int x=delete_number+1;x<=num;x++){
		lseek( fd , x*sizeof(Question) , SEEK_SET );
		read( fd , &question , sizeof(Question) );
		lseek( fd , -2*sizeof(Question) , SEEK_CUR );
		write( fd , &question , sizeof(Question) );
	}
}

void delete_status(){
	Status status;
	for(int x=delete_number+1;x<=num;x++){
		lseek( fd , x*sizeof(Status) , SEEK_SET );
		read( fd , &status , sizeof(Status) );
		lseek( fd , -2*sizeof(Status) , SEEK_CUR );
		write( fd , &status , sizeof(Status) );
	}
}

int main( int argc , char** argv ){
	fprintf( stderr , "[System] Please enter the type you want to delete.( human / bacteria / question )\n");
	sleep(1);
	fprintf( stderr , "[System] Your answer:");

	type = get_type();
	fd = open_log();
	num = get_num();

	if( type == -1){
		fprintf( stderr , "[System] Bug. Please notify the productor.\n");
		return 0;
	}
	if( num == 0 ){
		fprintf( stderr , "[System] %s has nothing to delete.\n", type_name[type] );
		return 0;
	}
	
	sleep(1);
	fprintf( stderr , "[System] These are your database.\n");
	sleep(1);
	show_log();
	sleep(1);
	fprintf( stderr , "[System] Please enter a number you want to delete.(#?)\n");
	sleep(1);
	fprintf( stderr , "[System] Your answer:");

	delete_number = get_delete_number();

	modify_num();
	if( type == 2 )                   delete_question();
	else if( type == 0 || type == 1 ) delete_status();
	else{
		fprintf( stderr , "[System] Bug. Please notify the productor.\n");
		return 0;
	}

	fprintf( stderr , "[System] These are your database.\n");
	sleep(1);
	show_log();
	fprintf( stderr , "[System] Bye bye~~~.\n");
	return 0;
}
