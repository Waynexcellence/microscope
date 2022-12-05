#include "general.c"

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
			fprintf( stderr , " #%d [Description] \e[7m%s\e[0m\n", x , question.description );
		}
		else if( type == 0 || type == 1 ){
			read( fd , &status , sizeof(Status) );
			fprintf( stderr , " #%d [Name] \e[7m%s\e[0m\n", x , status.name );
		}
		else	fprintf( stderr , "[System] Bug, Please notify the productor.\n");
	}
	fprintf( stderr , "\n");
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
	if( argc == 3 ){
		if( strcmp(type_name[0],argv[1]) == 0 ) type = 0;
		if( strcmp(type_name[1],argv[1]) == 0 ) type = 1;
		if( type == -1 ){
			fprintf( stderr , "[System] Fatal error, please notify the productor.\n");
			exit(0);
		}
		fd = open_log();
		num = get_num();
		sscanf( argv[2] , "%d" , &delete_number );
		modify_num();
		delete_status();
		return 0;
	}
	if( argc != 1 ){
		fprintf( stderr , "[System] Fatal error, please notify the productor.\n");
		exit(0);
	}
	fprintf( stderr , "[System] Please enter the type you want to delete.\e[4m( human / bacteria / question )\e[0m\n");
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
	
	fprintf( stderr , "[System] These are your database.\n");
	show_log();
	fprintf( stderr , "[System] Please enter a number you want to delete.\e[4m(#?)\e[0m\n");
	fprintf( stderr , "[System] Your answer:");
	
	delete_number = get_user_number();
	while( delete_number >= num || delete_number < 0 ){
		fprintf( stderr , "[System] Please checck your number range.\n");
		delete_number = get_user_number();
	}

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
