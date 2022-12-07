#include "general.c"

int bacteria_fd = -1;
int human_fd = -1;
int question_fd = -1;
int human_num = -1;
int question_num = -1;
int attack_ID = -1;
int self_ID = -1;
int event_time = -1;

char self_ID_string[3];
char* self_name;
Status self_status;
Status human_status;

void get_human_num(){
	char buf[50] = {};
	lseek( human_fd , 10*sizeof(Status) , SEEK_SET );
	read( human_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &human_num );
}

void get_question_num(){
	char buf[50] = {};
	lseek( question_fd , 100*sizeof(Question) , SEEK_SET );
	read( question_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &question_num );
}

void open_log(){
	bacteria_fd = open( "./log/bacteria.txt" , O_RDWR );
	human_fd = open( "./log/human.txt" , O_RDWR );
	question_fd = open( "./log/question.txt" , O_RDWR );
	get_human_num();
	get_question_num();
}

void get_status(){
	lseek( bacteria_fd , self_ID*sizeof(Status) , SEEK_SET );
	read( bacteria_fd , &self_status , sizeof(Status) );
}

void store_human_data(){
	lseek( human_fd , attack_ID*sizeof(Status) , SEEK_SET );
	write( human_fd , &human_status , sizeof(Status) );
}

void store_bacteria_data(){
	lseek( bacteria_fd , self_ID*sizeof(Status) , SEEK_SET );
	write( bacteria_fd , &self_status , sizeof(Status) );
}

void show_self_status(){
	fprintf( stderr , "[System] These are %s's current status:\n", self_name );
	char buf[100] = {};
	int length = sprintf( buf , "|  HP:%d  |  ATK:%d  |  Defense:%d  |  money:%d  |", self_status.HP , self_status.ATK , self_status.Defense , self_status.money );
	char up_down[150] = {};
	memset( up_down , '-' , length );
	fprintf( stderr , "%s\n" , up_down );
	fprintf( stderr , "|\e[42m  HP:%d  \e[40m|\e[41m  ATK:%d  \e[40m|\e[46m  Defense:%d  \e[40m|\e[43m  Money:%d  \e[40m|\n", self_status.HP , self_status.ATK , self_status.ATK , self_status.money );
	fprintf( stderr , "%s\n" , up_down );
	fprintf( stderr , "\n");
	store_bacteria_data();
}

void show_human(){
	fprintf( stderr , "[System] These are all human information.\n");
	lseek( human_fd , 0 , SEEK_SET );
	Status temp;
	for(int x=0;x<human_num;x++){
		read( human_fd , &temp , sizeof(Status) );
		fprintf( stderr , " #%d :\e[7m%s\e[0m   \e[42mHP:%d\e[40m   \e[41mATK:%d\e[40m   \e[46mDefense:%d\e[40m\n", x , temp.name , temp.HP , temp.ATK , temp.Defense );
	}
	fprintf( stderr , "\n");
}
void human_attack( bool* end ){
	int attack_from_human = human_status.ATK-self_status.Defense;
	if( attack_from_human <= 0 ) attack_from_human = 0; 
	self_status.HP -= attack_from_human;
	fprintf( stderr , "[Event] You are attacked by %s \e[42m%d HP\e[40m\n" , human_status.name , attack_from_human );
	store_bacteria_data();
	if( self_status.HP <= 0 ){
		*end = true;
		fprintf( stderr , "[Event] You are defeated by %s.\n", human_status.name );
		int child_pid = fork();
		int child_status = -1;
		char attack_ID_string[10] = {};
		sprintf( attack_ID_string , "%d" , attack_ID );
		if( child_pid == 0 ) execl( "./delete" , "./delete" , "bacteria" , self_ID_string , NULL );
		wait( &child_status );
		exit(0) ;
	}
}

void bacteria_attack( bool* end ){
	int attack_to_human = self_status.ATK-human_status.Defense;
	attack_to_human *= ( attack_to_human < 0 )? 0 : 1;
	human_status.HP -= attack_to_human;
	fprintf( stderr , "[Event] You attack %s cause \e[42m%d HP\e[40m damage\n" , human_status.name , attack_to_human );
	store_human_data();
	if( human_status.HP <= 0 ){
		fprintf( stderr , "[Event] You successfully defeat %s.\n", human_status.name );
		*end = true;
		char attack_ID_string[10] = {};
		sprintf( attack_ID_string , "%d" , attack_ID );
		int child_status = -1;
		int child_pid = fork();
		if( child_pid == 0 ) execl( "./delete" , "./delete" , "human" , attack_ID_string , NULL );
		wait( &child_status );
		human_num --;
	}
}

void yummy(){
	fprintf( stderr , "[Event] Because Ustilago_esculenta_Henn attack water bamboo, so bamboo become taster:).\n");
/*                         1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21 */
/*                         |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |  */	
/*                         |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |  */	
/*                         |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |  */	
	fprintf( stderr , "    *             *\n");
	fprintf( stderr , "   ***           ***     *                      *             \n");
	fprintf( stderr , "  ** **         ** **    *                      *             \n");
	fprintf( stderr , " **   **       **   **   *                      *             \n");
	fprintf( stderr , "**     **     **     **  **** ****   * *   *    **** **** ****\n");
	fprintf( stderr , "                         *  * *  **  ** * * *   *  * *  * *  *\n");
	fprintf( stderr , "                         *  * *  *** **  ** **  *  * *  * *  *\n");
	fprintf( stderr , "                         **** **** * *    *  *  **** **** ****\n");
	fprintf( stderr , "   **                  *                                      \n");
	fprintf( stderr , "    ****            ****                                      \n");
	fprintf( stderr , "       **************                                         \n");
	printf("\e[34m%s" , " ");
	fprintf( stderr , "             \e[46m******\e[40m                                 *        \n");
	fprintf( stderr , "             \e[46m****\e[40m       *         ******            *  **    \n");
	fprintf( stderr , "             \e[46m***\e[40m       *          *    *        *******      \n");
	fprintf( stderr , "             \e[46m**\e[40m       *********   *    *          * * *      \n");
	fprintf( stderr , "             \e[46m*\e[40m         *          *    *        **  *  *     \n");
	fprintf( stderr , "             \e[46m*\e[40m          *         ******           **   **   \n");
}

void attack(){
	lseek( human_fd , attack_ID*sizeof(Status) , SEEK_SET );
	read( human_fd , &human_status , sizeof(Status) );
	if( self_status.bacteria_race == Ustilago_esculenta_Henn && human_status.human_race == Bamboo ){
		yummy();
	}
	int order = rand()%2;
	bool end = false;
	if( order ){
		bacteria_attack( &end );
		if( end ) return;
		human_attack( &end );
	}
	else{
		human_attack( &end );
		if( end ) return;
		bacteria_attack( &end );
	}
	fprintf( stderr , "\n" );
}

int main(int argc , char** argv){
	// argv[1] is name
	// argv[2] is ID
	srand( time(NULL) );
	self_name = (char*) calloc( 100 , sizeof(char) );
	strcpy( self_name , argv[1] );               // self_name
	sscanf( argv[2] , "%d" , &self_ID );         // self_ID
	sprintf( self_ID_string , "%d" , self_ID );  // self_ID_string
	open_log();                                  // log_fd
	get_status();                                // self_status
	
	fprintf( stderr , "[System] Connected to (bacteria)(#%d) (%s) successfully.\n", self_ID , self_name );
	show_self_status();
	
	bool playing = true;
	if( question_num == 0 && human_num == 0 ){
		fprintf( stderr , "[System] There is no human and question.\n");
		exit(0);
	}
	while( playing ){
		int event = rand()%3;  /* answer question */ /* battle with human */
		if( event >= 0 && event <= 1 && question_num > 0 ){
			int attribute = -1;
			bool answer_correctly = random_question( &attribute , 0 , 1 , question_num );
			if( answer_correctly ){
				int buff = rand()%6+5;
				if( attribute == 0 ){ fprintf( stderr , "[Reward] Your \e[42mHP\e[40m increase %d.\n", buff );      self_status.HP      += buff;}
				if( attribute == 1 ){ fprintf( stderr , "[Reward] Your \e[46mDefense\e[40m increase %d.\n", buff ); self_status.Defense += buff;}
				if( attribute == 2 ){ fprintf( stderr , "[Reward] Your \e[41mATK\e[40m increase %d.\n", buff );     self_status.ATK     += buff;}
			}
			else{
				int debuff = rand()%6+5;
				if( attribute == 0 ){ fprintf( stderr , "[WA] Your \e[42mHP\e[40m decrease %d.\n", debuff );      self_status.HP      -= debuff;}
				if( attribute == 1 ){ fprintf( stderr , "[WA] Your \e[46mDefense\e[40m decrease %d.\n", debuff ); self_status.Defense -= debuff;}
				if( attribute == 2 ){ fprintf( stderr , "[WA] Your \e[41mATK\e[40m decrease %d.\n", debuff );     self_status.ATK     -= debuff;}
			}
			show_self_status();
			if( self_status.HP <= 0 ){
				fprintf( stderr , "[System] You looks unable to adapt to the environment well.\n");
				fprintf( stderr , "[System] Expection with next time you come to the world.\n");
				int child_status = -1;
				int child_pid = fork();
				if( child_pid == 0 ) execl( "./delete" , "./delete" , "bacteria" , self_ID_string , NULL );
				wait( &child_status );
				exit(0);
			}
			store_bacteria_data();
		}
		if( event >= 2 && event <= 2 && human_num > 0 ){
			fprintf( stderr , "[Event] You have to battle with a human.\n");
			show_human( 0 );
			fprintf( stderr , "[Event] Please select a human to attack.\e[4m(#?)\e[0m\n");
			fprintf( stderr , "[Event] Your answer:");
			attack_ID = get_user_number();
			while( attack_ID >= human_num || attack_ID < 0 ){
				fprintf( stderr , "[System] Please check your number range.\n");
				fprintf( stderr , "[System] Your answer:");
				attack_ID = get_user_number();
			}
			attack();
		}
	}
	


	return 0;
}
