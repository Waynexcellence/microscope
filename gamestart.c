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

char bacteria_exe_name[20] = "./bacteria";
char human_exe_main[20] = "./human";
char human_info[20] = "./human_info.txt";
char bacteria_info[20] = "./bacteria_info.txt";
char* character_input;
char* enter_name;
char* new_ID;
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
		read( STDIN_FILENO , buf , sizeof(buf) );
		int length = strlen(buf);
		buf[length-1] = 0;
		for(int x=0;x<length-1;x++){
			buf[x] = tolower(buf[x]);
		}
		if( strcmp( buf , human ) == 0 ) break;
		if( strcmp( buf , bacteria ) == 0 ) break;
		fprintf( stderr , "\nCharacter error please enter again.\n");
		fprintf( stderr , "Your answer:");
	}
	sprintf( character_input , "%s" , buf );
}

void get_num_account(){
	char buf[10] = {};
	lseek( info_fd , 0 , SEEK_SET );
	read( info_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &num_account );
}

void show_account( char* character ){
	fprintf( stderr , "This is your %s account\n", character );
	if( num_account == 0 ){
		fprintf( stderr , "#0 : \n");
		fprintf( stderr , "#1 : \n");
	}
	char buf[200] = {};
	for(int x=0;x<num_account;x++){
		memset( buf , '\0' , 200 );
		lseek( info_fd , 100*(x+1) , SEEK_SET );
		read( info_fd , buf , sizeof(buf) );
		fprintf( stderr , "#%d : %s\n", x , buf );
		strcpy( total_name[x] , buf );
	}
	fprintf( stderr , "\n" );
}

void get_mode(){
	char buf[100] = {};
	while( true ){
		read( STDIN_FILENO , buf , sizeof(buf) );
		int length = strlen(buf);
		buf[length-1] = 0;
		if( length == 2 && buf[0] >= '0' && buf[0] <= '1' ) break;
		fprintf( stderr , "mode select error please enter again.\n");
		fprintf( stderr , "Your answer:");
	}
	mode = buf[0] - '0';
}

void get_new_ID(){
	char buf[1000] = {};
	int bytes = -1;
	while( true ){
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		if( bytes <= 80 ) break;
		fprintf( stderr , "name invalid please enter again.\n");
		fprintf( stderr , "The new name:");
	}
	buf[bytes-1] = 0;
	strcpy( new_ID , buf );
}

void push_new_ID(){
	off_t position = 100*(1+num_account);
	lseek( info_fd , position , SEEK_SET );
	write( info_fd , new_ID , strlen(new_ID) );
	num_account ++;
	char buf[2] = {};
	sprintf( buf , "%d" , num_account );
	lseek( info_fd , 0 , SEEK_SET );
	write( info_fd , buf , strlen(buf) );
}

void get_enter_ID(){
	char buf[100] = {};
	int bytes = -1;
	while( true ){
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		for(int x=0;x<num_account;x++){
			if( strcmp( total_name[x] , buf ) == 0 ){
				enter_ID = x;
				break;
			}
		}
		if( enter_ID != -1 ) break;
		fprintf( stderr , "Sorry no such ID found please enter again.\n");
		fprintf( stderr , "Enter your ID:");
	}
}

int main(){
	character_input = (char*) calloc( 100 , sizeof(char) );
	new_ID          = (char*) calloc( 100 , sizeof(char) );
	total_name      = (char**) calloc( 10 , sizeof(char*) );
	for(int x=0;x<10;x++){
		total_name[x] = (char*) calloc( 100 , sizeof(char) );
	}

	printf("\n");
	sleep(1);
	fprintf( stderr , "Welcome to the Educcation Simulation Game.\n");
	sleep(1);
	fprintf( stderr , "Please enter the character you want to play: (human/bacteria)\n");
	sleep(1);
	fprintf( stderr , "Your answer:");

	get_character_input();
	fprintf( stderr , "\n");

	if( character_input[0] == 'h' ) info_fd = open( human_info , O_RDWR );
	else                            info_fd = open( bacteria_info , O_RDWR );
	
	get_num_account();
	show_account( character_input );
	
	sleep(1);
	fprintf( stderr , "Now you want to play as a %s, you need to (log in / create a new role)\n", character_input );
	sleep(1);
	fprintf( stderr , "0  for ( log in )\n");
	sleep(1);
	fprintf( stderr , "1  for ( create )\n");
	sleep(1);
	fprintf( stderr , "Your answer:");

	get_mode();
	
	//create a new account
	if( mode == 1 ){
		if( num_account == 10 ){
			fprintf( stderr , "Sorry you have too many account.\n");
			return 0;
		}
		fprintf( stderr , "Please enter the name you want to call your new character:(less 50)\n");
		fprintf( stderr , "The new name:");
		
		get_new_ID();
		push_new_ID();
		fprintf( stderr , "\n");
		show_account( character_input );
		return 0;
	}

	//log in
	if( num_account == 0){
		fprintf( stderr , "Sorry you don't have any account yet.\n");
		fprintf( stderr , "Please create an new account first.\n");
		return 0;
	}
	fprintf( stderr , "Please enter a character ID to log in.\n");
	fprintf( stderr , "Enter your ID:");

	get_enter_ID();
	fprintf( stderr , "enter (%d)\n" , enter_ID );

	return 0;
}
