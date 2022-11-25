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

int self_ID = -1;
int log_fd = -1;
int event_time = 0;
char* self_name;
Status status;

void show_self_status(){
	fprintf( stderr , "[System] These are %s's current status:\n", self_name );
	char buf[100] = {};
	int length = sprintf( buf , "|  HP:%d  |  ATK:%d  |  Defense:%d  |  money:%d  |", status.HP , status.ATK , status.Defense , status.money );
	char up_down[150] = {};
	memset( up_down , '-' , length );
	fprintf( stderr , "%s\n" , up_down );
	fprintf( stderr , "%s\n" , buf );
	fprintf( stderr , "%s\n" , up_down );
}

void store_data(){
	lseek( log_fd , self_ID*sizeof(Status) , SEEK_SET );
	write( log_fd , &status , sizeof(Status) );
	perror("store");
}

void open_info(){
	char buf[100] = "./log/human_info.txt";
	log_fd = open( buf , O_RDWR );
}

void get_status(){
	lseek( log_fd , self_ID*sizeof(Status) , SEEK_SET );
	read( log_fd , &status , sizeof(Status) );
}

void discover_shop(){
	int attribute = rand()%3;
	int buff = rand()%10+5;
	int need_money = rand()%31+5;
	fprintf( stderr , "[Event] You encounter a shop.\n");
	sleep(1);
	if( attribute == 1 ){// Defense
		fprintf( stderr , "[Event] The businessman sell a vaccine($%d) to strength your Defense(%d).(Yes/No)\n", need_money , buff );
	}
	else if( attribute == 2 ){// HP
		char shop_class[3][20] = { "department store" , ">////<" , "theater" };
		int class = rand()%3;
		fprintf( stderr , "[Event] It's a %s, you can enjoy a comfortable time($%d) to improve your HP(%d).(Yes/No)\n", shop_class[class] , need_money , buff );
	}
	else{// money
		fprintf( stderr , "[Event] It's time for buying a lottery ticket.\n");
		fprintf( stderr , "[Event] Which one do you like to buy?\n");
		fprintf( stderr , "[Event] A($5)  B($10)  C($20)  D(No)\n");
		fprintf( stderr , "[Event] Your answer:");
		char choose = get_choose();
		fprintf( stderr , "\n");
		if( choose == 'D' ){
			fprintf( stderr , "[Event] end\n");
			return ;
		}
		need_money = (choose=='A')? 5 : (choose=='B')? 10 : (choose=='C')? 20 : 0;
		if( status.money < need_money ){
			fprintf( stderr , "[Event] So sad you don't have enough money.\n");
			fprintf( stderr , "[Event] end.\n");
			return ;
		}
		buff = (choose=='A')? rand()%20+1 : (choose=='B')? rand()%26+5 : (choose=='C')? rand()%61+10 : 0;
		if( buff > need_money )  fprintf( stderr , "[Event] Congratulation! Now you have more fantastic coin!\n");
		if( buff == need_money ) fprintf( stderr , "[Event] Very good. You make both ends meet.\n");
		if( buff < need_money )  fprintf( stderr , "[Event] Not bad. Wish you have a nice day!\n");
		status.money += buff-need_money;
		show_self_status();
		store_data();
		fprintf( stderr , "[Event] end.\n");
		return ;
	}

	fprintf( stderr , "[Event] Your answer:");
	if( get_reply() ){
		if( status.money < need_money ){
			fprintf( stderr , "[Event] So sad you don't have enough money.\n");
			return ;
		}
		if( attribute == 1 )      status.Defense += buff;
		else if( attribute == 2 ) status.ATK += buff;
		status.money -= need_money;
		fprintf( stderr , "[Event] Congratulation! You have been stronger!\n");
		show_self_status();
	}
	fprintf( stderr , "[Event] end\n\n");
}

int main(int argc , char** argv){
	// argv[1] is name
	// argv[2] is ID
	self_name = (char*) calloc( 100 , sizeof(char) );
	strcpy( self_name , argv[1] );       // self_name
	sscanf( argv[2] , "%d" , &self_ID ); // self_ID
	open_info();                         // log_fd
	get_status();                        // self_status

	fprintf( stderr , "[System] Connect to (human)(#%d) (%s) successfully\n\n", self_ID , self_name );
	srand( time(NULL) );
	sleep(2);
	show_self_status();

	bool playing = true;

	while( playing ){
		int event = rand()%5;
		if( event == 0 ){
			playing = false;
			break;
		}
		if( event <= 2 ){// 1 2
			discover_shop();
		}
		if( event >= 3 ){// 3 4
			bool answer_correctly = random_question();
		}
		event_time ++;
	}


	store_data();
	
	return 0;
}
