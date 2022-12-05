#include "general.c"

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
