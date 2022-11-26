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
#include <limits.h>
#include <time.h>

int log_fd = -1;
int self_ID = -1;
char* self_name;
Status self_status;

void open_log(){
	char buf[100] = "./log/bacteria.txt";
	log_fd = open( buf , O_RDWR );
	perror("open");
}

void get_status(){
	lseek( log_fd , self_ID*sizeof(Status) , SEEK_SET );
	read( log_fd , &self_status , sizeof(Status) );
}

int main(int argc , char** argv){
	// argv[1] is name
	// argv[2] is ID
	self_name = (char*) calloc( 100 , sizeof(char) );
	strcpy( self_name , argv[1] );               // self_name
	sscanf( argv[2] , "%d" , &self_ID );         // self_ID
	open_log();                                  // log_fd
	get_status();                                // self_status
	
	fprintf( stderr , "[System] Connected to (bacteria)(#%d) (%s) successfully.\n", self_ID , self_name );
	
	
	return 0;
}
