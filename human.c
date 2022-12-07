#include "general.c"

int self_ID = -1;
int human_fd = -1;
int bacteria_fd = -1;
int question_fd = -1;
int bacteria_num = -1;
int question_num = -1;
int attack_ID = -1;
int event_time = 0;
char self_ID_string[3];
char* self_name;
Status self_status;
Status bacteria_status;
char good_talk[3][30] = { "Congratulation!" , "Great!" , "Fantastic!" };

void store_human_data(){
	lseek( human_fd , self_ID*sizeof(Status) , SEEK_SET );
	write( human_fd , &self_status , sizeof(Status) );
}

void store_bacteria_data(){
	lseek( bacteria_fd , attack_ID*sizeof(Status) , SEEK_SET );
	write( bacteria_fd , &bacteria_status , sizeof(Status) );
}

void show_self_status(){
	fprintf( stderr , "[System] These are \e[7m%s\e[0m's current status:\n", self_name );
	char buf[100] = {};
	int length = sprintf( buf , "|  HP:%d  |  ATK:%d  |  Defense:%d  |  money:%d  |", self_status.HP , self_status.ATK , self_status.Defense , self_status.money );
	char up_down[150] = {};
	memset( up_down , '-' , length );
	fprintf( stderr , "%s\n" , up_down );
	fprintf( stderr , "|\e[42m  HP:%d  \e[40m|\e[41m  ATK:%d  \e[40m|\e[46m  Defense:%d  \e[40m|\e[43m  Money:%d  \e[40m|\n", self_status.HP , self_status.ATK , self_status.Defense , self_status.money );
	fprintf( stderr , "%s\n" , up_down );
	fprintf( stderr , "\n");
	store_human_data();
}

void get_bacteria_num(){
	char buf[50] = {};
	lseek( bacteria_fd , 10*sizeof(Status) , SEEK_SET );
	read( bacteria_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &bacteria_num );
}

void get_question_num(){
	char buf[50] = {};
	lseek( question_fd , 100*sizeof(Question) , SEEK_SET );
	read( question_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &question_num );
}

void open_log(){
	char buf[100] = {};
	sprintf( buf , "./log/human.txt" );
	human_fd = open( buf , O_RDWR );
	sprintf( buf , "./log/bacteria.txt" );
	bacteria_fd = open( buf , O_RDWR );
	get_bacteria_num();
	question_fd = open( "./log/question.txt" , O_RDWR );
	get_question_num();
}

void get_status(){
	lseek( human_fd , self_ID*sizeof(Status) , SEEK_SET );
	read( human_fd , &self_status , sizeof(Status) );
}

void show_bacteria( int show_mode ){
	lseek( bacteria_fd , 0 , SEEK_SET );
	Status temp;
	fprintf( stderr , "[System] These are all your bacteria account.\n\n");
	for(int x=0;x<bacteria_num;x++){
		read( bacteria_fd , &temp , sizeof(Status) );
		if( show_mode == 0 ) fprintf( stderr , "#%d \e[7m%s\e[0m\n" , x , temp.name );
		if( show_mode == 1 ) fprintf( stderr , "#%d \e[7m%s\e[0m \e[42mHP:%d\e[40m \e[41mATK:%d\e[40m \e[46mDefense:%d\e[40m\n" , x , temp.name , temp.HP , temp.ATK , temp.Defense );
	}
	fprintf( stderr , "\n");
}

void discover_shop(){
	int attribute = rand()%4+1;
	int buff = rand()%10+5;
	int need_money = rand()%31+5;
	fprintf( stderr , "[Event] You encounter a shop.\n");
	if( attribute == 1 ){// Defense
		fprintf( stderr , "[Event] The businessman sell a \e[43mvaccine($%d)\e[40m to strength your \e[46mDefense(%d)\e[40m.\e[4m(Yes/No)\e[0m\n", need_money , buff );
	}
	else if( attribute == 2 ){// ATK
		char shop_class[3][20] = { "department store" , ">////<" , "theater" };
		int class = rand()%3;
		fprintf( stderr , "[Event] It's a %s, you can enjoy a comfortable time\e[43m($%d)\e[40m to improve your \e[41mATK(%d)\e[40m.\e[4m(Yes/No)\e[0m\n", shop_class[class] , need_money , buff );
	}
	else if( attribute == 3 ){// see bacteria status
		need_money *= rand()%2+2;
		need_money += rand()%31+20;
		fprintf( stderr , "[Event] You can cost \e[43m(%d)\e[40m money buying a microscope to see the bacteria information.\e[4m(Yes/No)\e[0m\n", need_money );
	}
	else{// money
		fprintf( stderr , "[Event] It's time for buying a lottery ticket.\n");
		fprintf( stderr , "[Event] Which one do you like to buy?\n");
		fprintf( stderr , "[Event] A(\e[43m$5\e[40m)  B(\e[43m$10\e[40m)  C(\e[43m$20\e[40m)  D(No)\n");
		fprintf( stderr , "[Event] Your answer:");
		char choose = get_choose();
		fprintf( stderr , "\n");
		if( choose == 'D' ){
			return ;
		}
		need_money = (choose=='A')? 5 : (choose=='B')? 10 : (choose=='C')? 20 : 0;
		if( self_status.money < need_money ){
			fprintf( stderr , "[Event] So sad you don't have enough money.\n");
			return ;
		}
		buff = (choose=='A')? rand()%20+1 : (choose=='B')? rand()%26+5 : (choose=='C')? rand()%61+10 : 0;
		fprintf( stderr , "[Event] You get \e[43m%d\e[40m as a reward.\n", buff );
		self_status.money += buff-need_money;
		show_self_status();
		return ;
	}
	
	fprintf( stderr , "[Event] Your answer:");
	if( get_reply() ){
		fprintf( stderr , "\n");
		if( self_status.money < need_money ){
			fprintf( stderr , "[Event] So sad you don't have enough money.\n");
			return ;
		}
		self_status.money -= need_money;
		if( attribute == 1 ) self_status.Defense += buff;
		if( attribute == 2 ) self_status.ATK += buff;
		if( attribute >= 1 && attribute <= 2 ){
			fprintf( stderr , "[Event] You are now stronger!\n");
			show_self_status();
		}
		if( attribute == 3 ){
			show_bacteria( 1 );
		}
	}
	else fprintf( stderr , "\n");
}

void human_attack( bool* end ){
	int attack_to_bacteria = self_status.ATK-bacteria_status.Defense;
	attack_to_bacteria *= ( attack_to_bacteria < 0 )? 0 : 1;
	bacteria_status.HP -= attack_to_bacteria;
	fprintf( stderr , "[Event] You attack \e[7m%s\e[0m \e[42m%d HP\e[40m\n" , bacteria_status.name , attack_to_bacteria );
	store_bacteria_data();
	if( bacteria_status.HP <= 0 ){
		*end = true;
		int good = rand()%3;
		fprintf( stderr , "[Event] %s You defeat the \e[7m%s\e[0m bacteria.\n", good_talk[good] , bacteria_status.name );
		int child_pid = fork();
		int child_status = -1;
		char attack_ID_string[10] = {};
		sprintf( attack_ID_string , "%d" , attack_ID );
		if( child_pid == 0 ) execl( "./delete" , "./delete" , "bacteria" , attack_ID_string , NULL );
		wait( &child_status );
		bacteria_num --;
		return ;
	}
}

void HP_below_0( bool* end ){
	if( self_status.human_race != Patient ){
		*end = false;
		fprintf( stderr , "[Event] your \e[42mHP\e[40m is <0 so you become a \e[7mpatient\e[0m from now on.\n");
		fprintf( stderr , "[Event] Once you pass away again you will receive some punishment AWA\n");
		self_status.human_race = Patient;
		self_status.HP = rand()%5+5;
		self_status.ATK /= 2;
		self_status.Defense /= 2;
		self_status.money /= 2;
		show_self_status();
	}
	else{
		*end = true;
		fprintf( stderr , "[Event] QQ you are defeated by (%s) bacteria.\n", bacteria_status.name );
		fprintf( stderr , "[Event] Do you remember the punishment ^^ ?\n");
		sleep(1);
		fprintf( stderr , "[Event] three\n");
		sleep(1);
		fprintf( stderr , "[Event] two\n");
		sleep(1);
		fprintf( stderr , "[Event] one\n");
		sleep(1);
		fprintf( stderr , "[Event] Go\n");
		show_punishment();
		fprintf( stderr , "[Event] Bye bye~\n");
		int child_pid = fork();
		int child_status = -1;
		if( child_pid == 0 ) execl( "./delete" , "./delete" , "human" , self_ID_string , NULL );
		wait( &child_status );
		exit( 0 );
	}
}

void bacteria_attack( bool* end ){
	int attack_from_bacteria = bacteria_status.ATK-self_status.Defense;
	attack_from_bacteria *= ( attack_from_bacteria < 0 )? 0 : 1;
	self_status.HP -= bacteria_status.ATK-self_status.Defense;
	fprintf( stderr , "[Event] You are attacked by \e[7m%s\e[0m \e[42m%d HP\e[40m\n" , bacteria_status.name , attack_from_bacteria );
	show_self_status();
	if( self_status.HP <= 0 ){
		bool end = false;
		HP_below_0( &end );
	}
}

void attack(){
	lseek( bacteria_fd , attack_ID*sizeof(Status) , SEEK_SET );
	read( bacteria_fd , &bacteria_status , sizeof(Status) );
	int order = rand()%2;
	bool end = false;
	if( order ){
		human_attack( &end );
		if( end ) return;
		bacteria_attack( &end );
	}
	else{
		bacteria_attack( &end );
		if( end ) return;
		human_attack( &end );
	}
	fprintf( stderr , "\n" );
}

void bacteria_stronger(){
	Status temp;
	lseek( bacteria_fd , 0 , SEEK_SET );
	for(int x=0;x<bacteria_num;x++){
		read( bacteria_fd , &temp , sizeof(Status) );
		int buff = rand()%5+1;
		bool make_strong;
		make_strong = ( rand()%2 )? true : false;
		if( make_strong ) temp.HP += buff;
		make_strong = ( rand()%2 )? true : false;
		if( make_strong ) temp.ATK += buff;
		make_strong = ( rand()%2 )? true : false;
		if( make_strong ) temp.Defense += buff;
		lseek( bacteria_fd , -sizeof(Status) , SEEK_CUR );
		write( bacteria_fd , &temp , sizeof(Status) );
	}
}

int main(int argc , char** argv){
	// argv[1] is name
	// argv[2] is ID
	self_name = (char*) calloc( 100 , sizeof(char) );
	strcpy( self_name , argv[1] );              // self_name
	sscanf( argv[2] , "%d" , &self_ID );        // self_ID
	sprintf( self_ID_string , "%d" , self_ID ); // self_ID_string
	open_log();                                 // human_fd  bacteria_fd
	get_status();                               // self_status

	fprintf( stderr , "[System] Connect to (human)(#%d) (%s) successfully\n\n", self_ID , self_name );
	srand( time(NULL) );
	sleep(1);
	show_self_status();
	sleep(1);

	bool playing = true;

	while( playing ){
		int event = rand()%5+1;/* discover shop */ /* question */ /* battle with bacteria */
		if( event == 0 && event_time >= 10 ){
			fprintf( stderr , "[System] take a break first.\n");
			playing = false;
			break;
		}
		if( event >= 1 && event <= 2 ){// 1 2
			discover_shop();
		}
		if( event >= 3 && event <= 4  && question_num > 0 ){// 3 4
			int attribute = -1;
			bool answer_correctly = random_question( &attribute , 1 , 0 , question_num );
			if( answer_correctly ){
				int good_num = rand()%3;
				int buff = rand()%6+3;
				fprintf( stderr , "[Event] %s You answer correctly.\n", good_talk[good_num] );
				fprintf( stderr , "[Event] Your reward is \e[43m%d coin\e[40m.\n" , buff );
				self_status.money += buff;
				show_self_status();
			}
			else{
				int debuff = rand()%4+1;
				fprintf( stderr , "[Event] Getting a WA will lead to a punishment. AWA\n");
				if( attribute == 0 ){ fprintf( stderr , "[Event] Youe lose \e[42mHP %d\e[40m. AWA\n", debuff );      self_status.HP      -= debuff; }
				if( attribute == 1 ){ fprintf( stderr , "[Event] Youe lose \e[46mDefense %d\e[40m. AWA\n", debuff ); self_status.Defense -= debuff; }
				if( attribute == 2 ){ fprintf( stderr , "[Event] Youe lose \e[41mATK %d\e[40m. AWA\n", debuff );     self_status.ATK     -= debuff; }
				if( attribute == 3 ){ fprintf( stderr , "[Event] Youe \e[43mmoney\e[40m disappear \e[43m%d\e[40m. AWA\n", debuff );  self_status.money   -= debuff; }
				show_self_status();
				
				if( self_status.HP <= 0 ){
					bool end = false;
					HP_below_0( &end );
				}
			}
		}
		if( event >=5 && event <= 5 && bacteria_num > 0 ){
			fprintf( stderr , "[Event] You have to battle with a bacteria.\n");
			show_bacteria( 0 );
			fprintf( stderr , "[Event] Please select a bacteria to attack.\e[4m(#?)\e[0m\n");
			fprintf( stderr , "[Event] Your answer:");
			attack_ID = get_user_number();
			while( attack_ID >= bacteria_num || attack_ID < 0 ){
				fprintf( stderr , "[System] Please check your number range.\n");
				fprintf( stderr , "[System] Your answer:");
				attack_ID = get_user_number();
			}
			attack();
		}
		if( event_time % 5 == 4 ){
			bacteria_stronger();
		}
		event_time ++;
	}

	store_human_data();
	return 0;
}
