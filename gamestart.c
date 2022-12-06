#include "general.c"

char human_info[30] = "./log/human.txt";
char bacteria_info[30] = "./log/bacteria.txt";
char bamboo_string[10] = "bamboo";
char alone_black_bacteria[50] = "black";

char* character_input;
char* enter_name;
char* new_name;
char** total_name;

int log_fd = -1;
int num_account = -1;
int mode = -1;
int enter_ID = -1;

Status new_role;

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
		if( compare_exit(buf) ) exit( 0 );
		if( strcmp( buf , human ) == 0 ) break;
		if( strcmp( buf , bacteria ) == 0 ) break;
		fprintf( stderr , "[System] Character error please enter again.\n");
		fprintf( stderr , "[System] Your answer:");
	}
	sprintf( character_input , "%s" , buf );
}

void get_num_account(){
	char buf[10] = {};
	lseek( log_fd , 10*sizeof(Status) , SEEK_SET );
	read( log_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &num_account );
}

void show_account(){
	fprintf( stderr , "[System] This is your %s account list,\n", character_input );
	if( num_account == 0 ){
		fprintf( stderr , "[System] #0 : \n");
		fprintf( stderr , "[System] #1 : \n");
	}
	Status temp;
	lseek( log_fd , 0 , SEEK_SET );
	for(int x=0;x<num_account;x++){
		read( log_fd , &temp , sizeof(Status) );
		fprintf( stderr , "[System] \e[7m#%d : %s\e[0m\n", x , temp.name );
		strcpy( total_name[x] , temp.name );
	}
	fprintf( stderr , "\n" );
}

void get_mode(){
	int temp = -1;
	while( true ){
		temp = get_user_number();
		if( temp < 0 || temp > 1 ){
			fprintf( stderr , "[System] Please check your number range.\n");
			fprintf( stderr , "[System] Your answer:");
			continue;
		}
		break;
	}
	mode = temp;
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

bool check_special_string( char* buf , char* special ){
	int special_length = strlen(special);
	int length = strlen(buf);
	if( length < special_length ) return false;
	for(int x=0;x<=length-special_length;x++){
		char temp[30] = {};
		for(int y=x;y<x+special_length;y++){
			temp[y-x] = tolower( buf[y] );
		}
		if( strcmp( special , temp ) == 0 ) return true;
	}
	return false;
}

void push_new_name(){
	Status temp;
	strcpy( temp.name , new_name );
	if( character_input[0] == 'h' && check_special_string( temp.name , bamboo_string) )              temp.human_race = Bamboo;
	else                                                                                             temp.human_race = Common;
	if( character_input[0] == 'b' && check_special_string( temp.name , alone_black_bacteria) ) temp.bacteria_race = Ustilago_esculenta_Henn;
	else                                                                                             temp.bacteria_race = virus;
	temp.HP = rand()%91+10;
	temp.ATK = rand()%21+10;
	temp.Defense = rand()%11+10;
	temp.money = rand()%91+10;
	lseek( log_fd , num_account*sizeof(Status) , SEEK_SET );
	write( log_fd , &temp , sizeof(Status) );
	num_account ++;
	char buf[5] = {};
	sprintf( buf , "%d" , num_account );
	lseek( log_fd , 10*sizeof(Status) , SEEK_SET );
	write( log_fd , buf , strlen(buf) );
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
		if( compare_exit(buf) ) exit( 0 );
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
	fprintf( stderr , "[System] Please enter the character you want to play: \e[4m(human/bacteria)\e[0m\n");
	fprintf( stderr , "[System] Your answer:");
	get_character_input();
	if( character_input[0] == 'h' ) log_fd = open( human_info , O_RDWR );
	else                            log_fd = open( bacteria_info , O_RDWR );
	
	fprintf( stderr , "\n" );
	get_num_account();
	show_account();
	
	fprintf( stderr , "[System] Now you want to play as a %s, you need to (log in / create a new role)\n", character_input );
	fprintf( stderr , "[System] \e[4m0  for ( log in )\e[0m\n");
	fprintf( stderr , "[System] \e[4m1  for ( create )\e[0m\n");
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

		fprintf( stderr , "[System] Do you want to log in?\e[4m(yes/no)\e[0m\n");
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
	fprintf( stderr , "[System] Please enter \e[4ma character name\e[0m to log in.\n");
	fprintf( stderr , "[System] Your name:");
	
	get_enter_ID();
	fprintf( stderr , "\n");

	int pid = fork();
	int child_status = -1;
	if( pid == 0 ){
		fprintf( stderr , "[System] Logging in (%s)(#%d) (%s) ............\n", character_input , enter_ID , enter_name );
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
