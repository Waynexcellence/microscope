#include "status.h"
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
#include <time.h>
#include <limits.h>

int main(){

	int fd = -1;
	char buf[5] = "0";
	fd = open( "./log/human.txt" , O_RDWR | O_CREAT | O_TRUNC , 0744 );
	lseek( fd , 10*sizeof(Status) , SEEK_SET );
	write( fd , buf , strlen(buf) );

	fd = open( "./log/bacteria.txt" , O_RDWR | O_CREAT | O_TRUNC , 0744 );
	lseek( fd , 10*sizeof(Status) , SEEK_SET );
	write( fd , buf , strlen(buf) );
	
	perror("human and bacteria reset");
	
	fd = open( "./log/question.txt" , O_RDWR | O_CREAT | O_TRUNC , 0744 );
	lseek( fd , 100*sizeof(Question) , SEEK_SET );
	write( fd , buf , strlen(buf) );

	perror("question reset");
	return 0;
}
