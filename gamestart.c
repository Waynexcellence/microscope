#include "status.h"
#include "general.c"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>

char human_info[30] = "./log/human_info.txt";
char bacteria_info[30] = "./log/bacteria_info.txt";

char* character_input;
char* enter_name;
char* new_name;
char** total_name;

int info_fd = -1;
int num_account = -1;
int mode = -1;
int enter_ID = -1;

void get_character_input(){
	char buf[1000] = {};
	char human[10] = "human";
	char bacteria[10] = "bacteria";
	while( true ){
		memset( buf , '\0' , 1000 );
		int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		for(int x=0;x<bytes-1;x++){
			buf[x] = tolower(buf[x]);
		}
		if( compare_exit(buf) ){
			fprintf( stderr , "[System] Bye bye~~~\n");
			exit( 0 );
		}
		if( strcmp( buf , human ) == 0 ) break;
		if( strcmp( buf , bacteria ) == 0 ) break;
		fprintf( stderr , "[System] Character error please enter again.\n");
		fprintf( stderr , "[System] Your answer:");
	}
	sprintf( character_input , "%s" , buf );
}

void get_num_account(){
	char buf[10] = {};
	lseek( info_fd , 10*sizeof(Status) , SEEK_SET );
	read( info_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &num_account );
}

void show_account(){
	fprintf( stderr , "[System] This is your %s account list,\n", character_input );
	if( num_account == 0 ){
		fprintf( stderr , "[System] #0 : \n");
		fprintf( stderr , "[System] #1 : \n");
	}
	Status temp;
	sleep(1);
	lseek( info_fd , 0 , SEEK_SET );
	for(int x=0;x<num_account;x++){
		read( info_fd , &temp , sizeof(Status) );
		fprintf( stderr , "[System] #%d : %s\n", x , temp.name );
		strcpy( total_name[x] , temp.name );
	}
	fprintf( stderr , "\n" );
}

void get_mode(){
	char buf[100] = {};
	int bytes = -1;
	while( true ){
		memset( buf , '\0' , 100 );
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		if( compare_exit(buf) ){
			fprintf( stderr , "[System] Bye bye~~~\n");
			exit( 0 );
		}
		if( bytes == 2 && buf[0] >= '0' && buf[0] <= '1' ) break;
		fprintf( stderr , "[System] Mode select error please enter again.\n");
		fprintf( stderr , "[System] Your answer:");
	}
	mode = buf[0] - '0';
}

void get_new_name(){
	char buf[1000] = {};
	while( true ){
		memset( buf , '\0' , 1000 );
		int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		char temp[1000] = {};
		strcpy( temp , buf );
		for(int x=0;x<bytes-1;x++){
			temp[x] = tolower( temp[x] );
		}
		if( compare_exit(buf) ){
			fprintf( stderr , "[System] Sorry, exit can't not be name, you can exit with Ctrl+C, please enter again.\n");
			fprintf( stderr , "[System] The new name:");
			continue;
		}
		bool the_same = false;
		for(int x=0;x<num_account;x++){
			if( strcmp( buf , total_name[x] ) == 0 ){
				fprintf( stderr , "[System] Sorry the name is the same as #%d.\n", x );
				fprintf( stderr , "[System] Please enter again.\n");
				fprintf( stderr , "[System] The new name:");
				the_same = true;
				break;
			}
		}
		if( the_same ) continue;
		if( bytes <= 90 ) break;
		fprintf( stderr , "[System] Name invalid please enter again.\n");
		fprintf( stderr , "[System] The new name:");
	}
	strcpy( total_name[num_account] , buf );
	strcpy( new_name , buf );
}

void push_new_name(){
	Status temp;
	strcpy( temp.name , new_name );
	temp.HP = rand()%91+10;
	temp.ATK = rand()%21+10;
	temp.Defense = rand()%11+10;
	temp.money = rand()%91+10;
	lseek( info_fd , num_account*sizeof(Status) , SEEK_SET );
	write( info_fd , &temp , sizeof(Status) );
	num_account ++;
	char buf[5] = {};
	sprintf( buf , "%d" , num_account );
	lseek( info_fd , 10*sizeof(Status) , SEEK_SET );
	write( info_fd , buf , strlen(buf) );
}

void get_enter_ID(){
	char buf[1000] = {};
	while( true ){
		memset( buf , '\0', 1000 );
		int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		char temp[100] = {};
		strcpy( temp , buf );
		for( int x=0;x<bytes-1;x++){
			temp[x] = tolower( temp[x] );
		}
		if( compare_exit(buf) ){
			fprintf( stderr , "[System] Bye bye~~~\n");
			exit( 0 );
		}
		for(int x=0;x<num_account;x++){
			if( strcmp( total_name[x] , buf ) == 0 ){
				enter_ID = x;
				break;
			}
		}
		if( enter_ID != -1 ) break;
		fprintf( stderr , "[System] Sorry no such name found please enter again.\n");
		fprintf( stderr , "[System] Enter your name:");
	}
	strcpy( enter_name , buf );
}

int main(){
	srand( time(NULL) );
	character_input = (char*) calloc( 100 , sizeof(char) );
	new_name        = (char*) calloc( 100 , sizeof(char) );
	enter_name      = (char*) calloc( 100 , sizeof(char) );
	total_name      = (char**) calloc( 10 , sizeof(char*) );
	for(int x=0;x<10;x++){
		total_name[x] = (char*) calloc( 200 , sizeof(char) );
	}

	printf("\n");
	sleep(1);
	fprintf( stderr , "[System] Welcome to the Educcation Simulation Game.\n");
	sleep(1);
	fprintf( stderr , "[System] Please enter the character you want to play: (human/bacteria)\n");
	sleep(1);
	fprintf( stderr , "[System] Your answer:");
	get_character_input();
	if( character_input[0] == 'h' ) info_fd = open( human_info , O_RDWR );
	else                            info_fd = open( bacteria_info , O_RDWR );
	
	fprintf( stderr , "\n" );
	sleep(1);
	get_num_account();
	show_account();
	
	sleep(1);
	fprintf( stderr , "[System] Now you want to play as a %s, you need to (log in / create a new role)\n", character_input );
	fprintf( stderr , "[System] 0  for ( log in )\n");
	fprintf( stderr , "[System] 1  for ( create )\n");
	sleep(1);
	fprintf( stderr , "[System] Your answer:");

	get_mode();
	fprintf( stderr , "\n" );
	
	//create a new account
	if( mode == 1 ){
		if( num_account == 10 ){
			fprintf( stderr , "[System] Sorry you have too many account.\n");
			return 0;
		}
		fprintf( stderr , "[System] Please enter the name you want to call your new character:(less 50)\n");
		fprintf( stderr , "[System] The new name:");
		
		get_new_name();
		push_new_name();
		fprintf( stderr , "\n");
		show_account();

		fprintf( stderr , "[System] Do you want to log in?(yes/no)\n");
		fprintf( stderr , "[System] Your answer:");
		
		bool continue_enter = get_reply();
		if( !continue_enter ){
			fprintf( stderr , "[System] Bye bye~~~\n");
			return 0;
		}

	}

	//log in
	if( num_account == 0){
		fprintf( stderr , "[System] Sorry you don't have any account yet.\n");
		fprintf( stderr , "[System] Please create an new account first.\n");
		return 0;
	}
	fprintf( stderr , "[System] Please enter a character name to log in.\n");
	fprintf( stderr , "[System] Your name:");
	
	get_enter_ID();
	
	int pid = fork();
	int child_status = -1;
	if( pid == 0 ){
		fprintf( stderr , "[System] Logging in (%s)(#%d) (%s)......\n", character_input , enter_ID , enter_name );
		sleep(1);
		char exe_name[30] = {};
		char enter_ID_string[5] = {};
		sprintf( exe_name , "./%s" , character_input );
		sprintf( enter_ID_string , "%d" , enter_ID );
		execl( exe_name , exe_name , enter_name  , enter_ID_string  , NULL );
	}
	wait( &child_status );

	fprintf( stderr , "[System] Thanks %s for your playing. Good luck.\n", enter_name );
	return 0;
}
