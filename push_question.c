#include "general.c"

Question question;
int question_fd = -1;
int question_num = -1;

void get_question_num(){
	char buf[10] = {};
	lseek( question_fd , 100*sizeof(Question) , SEEK_SET );
	read( question_fd , buf , sizeof(buf) );
	sscanf( buf , "%d" , &question_num );
}

void get_fd(){
	question_fd = open( "./log/question.txt" , O_RDWR );
}

void get_question_type(){
	int temp = get_user_number();
	while( temp != 2 && temp != 4 ){
		fprintf( stderr , "[System] Please check your number range.\n");
		fprintf( stderr , "[System] Your answer:");
		temp = get_user_number();
	}
	question.type = temp;
}

void get_question_description(){
	char buf[100] = {};
	int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
	buf[bytes-1] = 0;
	strcpy( question.description , buf );
}

void get_check_option( int finish ){
	char buf[50] = {};
	while( true ){
		bool the_same = false;
		int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		for(int x=0;x<=finish;x++){
			if( strcmp( question.option[x] , buf ) == 0 ){
				the_same = true;
				break;
			}
		}
		if( !the_same ) break;
		fprintf( stderr , "[System] Your option has repeated. Please enter again.\n");
		fprintf( stderr , "[System] Your question option is:");
	}
	strcpy( question.option[finish+1] , buf );
}

void modify_num(){
	char buf[5] = {};
	sprintf( buf , "%d" , question_num );
	lseek( question_fd , 100*sizeof(Question) , SEEK_SET );
	write( question_fd , buf , strlen(buf) );
}

int main(){
	question.correct_answer = 0;
	
	get_fd();
	get_question_num();
	if( question_num >= 95 ){
		fprintf( stderr , "Too many question.\n");
		return 0;
	}
	fprintf( stderr , "\n");
	fprintf( stderr , "[System] You have been enrolled %d question.\n", question_num );
	fprintf( stderr , "[System] Please enter the question type.\n");
	fprintf( stderr , "[System] \e[4m2 for ( True/False )\e[0m\n");
	fprintf( stderr , "[System] \e[4m4 for  (  Select  )\e[0m\n");
	fprintf( stderr , "[System] Your question type:");
	
	get_question_type();

	fprintf( stderr , "[System] Please enter your \e[4mquestion description\e[0m.\n");
	fprintf( stderr , "[System] Your question description:");
	
	get_question_description();

	fprintf( stderr , "[Warning] \e[41mMake \e[42msure \e[43myour \e[44mfirst \e[45moption \e[46mis \e[40mthe correct answer, I will place them randomly when asking the question.\n");
	fprintf( stderr , "[System] You can Ctrl+C to interrupt the push_question and new question won't be pushed.\n");
	for(int x=0;x<question.type;x++){
		fprintf( stderr , "[System] Your %d'th question option is:", x );
		get_check_option( x-1 );
	}
	
	lseek( question_fd , question_num*sizeof(Question) , SEEK_SET );
	write( question_fd , &question , sizeof(Question) );

	question_num ++;
	
	modify_num();
	return 0;
}
