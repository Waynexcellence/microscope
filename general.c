#include "question.h"
#include "status.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


bool compare_exit( char* buf ){
	char Exit_string[10] = "exit";
	int length = strlen(buf);
	if( length != 4 )return false;
	for(int x=0;x<4;x++){
		buf[x] = tolower( buf[x] );
	}
	if( strcmp( Exit_string , buf ) == 0 ){
		fprintf( stderr , "[System] Bye bye~~~\n");
		return true;
	}
	return false;
}

bool get_reply(){
	char buf[100] = {};
	int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
	buf[bytes-1] = 0;
	if( compare_exit(buf) ) exit(0);
	if( buf[0] == 'y' || buf[0] == 'Y' ) return true;
	return false;
}

char get_choose(){
	char buf[100] = {};
	while( true ){
		bool pass = true;
		memset( buf , '\0' , 100 );
		int bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		if( compare_exit(buf) ) exit(0);
		if( bytes != 2 ){
			fprintf( stderr , "[Event] Only key a letter is enough.\n");
			pass = false;
		}
		if( buf[0] < 'A' || buf[0] > 'D' ){
			fprintf( stderr , "[Event] only ( A / B / C / D )\n");
			pass = false;
		}
		if( pass ) break;
		fprintf( stderr , "[Event] Please enter again.\n");
		fprintf( stderr , "[Event] Your answer:");
	}
	return buf[0];
}

int get_user_number(){
	char buf[100] = {};
	int bytes = -1;
	int temp = -1;
	while( true ){
		memset( buf , '\0' , 100 );
		bytes = read( STDIN_FILENO , buf , sizeof(buf) );
		buf[bytes-1] = 0;
		if( compare_exit(buf) ) exit(0);
		if( bytes < 0 ){
			fprintf( stderr , "[System] Fatal Error. Please notify the productor.\n");
			exit(0);
		}
		fprintf( stderr , "\n");
		sscanf( buf , "%d" , &temp );
		break;
	}
	return temp;
}

bool random_question( int* attribute , bool human , bool bacteria , int question_num ){
	char correct_answer = 'X';
	int fd = open( "./log/question.txt" , O_RDONLY );
	*attribute = ( human )? rand()%4 : rand()%3 ;
	int question_position = rand() % question_num;
	Question question;
	*attribute = bacteria;
	if( human ){
		if( *attribute == 0 ) fprintf( stderr , "[Event] Bumping into a Smallpox, your \e[42mHP\e[40m will be taken because it cause you ill if WA.\n");
		if( *attribute == 1 ) fprintf( stderr , "[Event] Bumping into a HIV, your \e[46mDefense\e[40m will be weaker because your immunity system is affected if WA.\n");
		if( *attribute == 2 ) fprintf( stderr , "[Event] Bumping into a Neurotropic, your \e[41mATK\e[40m will be less because of its nerve damage if WA.\n");
		if( *attribute == 3 ) fprintf( stderr , "[Event] Bumping into a COVID-19, your \e[43mmoney\e[40m will disappear because you buying many many mask if WA.\n");
	}
	if( bacteria ){
		if( *attribute == 0 ) fprintf( stderr , "[Event] Bumping into a T cell, your \e[42mHP\e[40m will be taken because it attack you if WA.\n");
		if( *attribute == 1 ) fprintf( stderr , "[Event] Bumping into a Antibiotic, your \e[46mDefense\e[40m will be weaker if WA.\n");
		if( *attribute == 2 ) fprintf( stderr , "[Event] Bumping into a Alcohol, your \e[41mATK\e[40m will be less if WA.\n");
	}
	sleep(2);

	lseek( fd , question_position*sizeof(Question) , SEEK_SET );
	read( fd , &question , sizeof(Question) );
	
	// random sort question option
	bool appeared[question.type];
	for(int x=0;x<question.type;x++){
		appeared[x] = false;
	}
	fprintf( stderr , "[Description] \e[7m%s\e[0m\n", question.description );
	fprintf( stderr , "[Option] ");
	for(int x=0;x<question.type;x++){
		char c = 'A' + x;
		int should_appear = -1;
		while( true ){
			should_appear = rand()%question.type;
			if( !appeared[should_appear] ) break;
		}
		if( should_appear == 0 ) correct_answer = 'A' + x;
		fprintf( stderr , "\e[4m(%c) %s\e[0m ", c , question.option[should_appear] );
		appeared[should_appear] = true;
	}
	sleep(1);
	fprintf( stderr , "\n");
	fprintf( stderr , "[Event] Your answer:");
	
	char self_answer = get_choose();
	fprintf( stderr , "\n");
	if( self_answer != correct_answer ){
		return false;
	}
	return true;
}

void show_punishment(){
/*                              |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    | */
/*                              |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    | */
/*                              |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    | */
	fprintf( stderr , "  **  ***         ****      ****   ****      ****        **   *** \n");
	fprintf( stderr , " *****   *      **   **      **     **     **   **     ****  *  **\n");
	fprintf( stderr , "  **     **    **     **      *     *     **     **       * *   **\n");
	fprintf( stderr , "  **     **    **      *      **    *     **      *       **      \n");
	fprintf( stderr , "  **     **    *       *      **   *      *       *       *       \n");
	fprintf( stderr , "  **     **    **********      *   *      **********      *       \n");
	fprintf( stderr , "  **     **    *               **  *      *               *       \n");
	fprintf( stderr , "  **     **    *               ** *       *               *       \n");
	fprintf( stderr , "  **     **    **               ***       **        *     *       \n");
	fprintf( stderr , "  **     **    **      *        ***       **       *      *       \n");
	fprintf( stderr , "  **     **     **    *         **         **     *       *       \n");
	fprintf( stderr , "  **     **       ****           *           ****      *******    \n");
	sleep(1);
	fprintf( stderr , "   ***** **      *****       **  ***       **  ***         *****   \n");
	fprintf( stderr , "  **   ** *     **   **     *****   *     *****   *      **    **  \n");
	fprintf( stderr , " **     *      **     **     **     **     **     **     *      *  \n");
	fprintf( stderr , " **     *      *       *     **     **     **     **     *      *  \n");
	fprintf( stderr , " **     *     **       **    **     **     **     **          ***  \n");
	fprintf( stderr , "  **   **     **       **    **     **     **     **       ***  *  \n");
	fprintf( stderr , "  ******      **       **    **     **     **     **     **     *  \n");
	fprintf( stderr , " **           **       **    **     **     **     **    **      *  \n");
	fprintf( stderr , " **            *       *     **     **     **     **    **      *  \n");
	fprintf( stderr , "  *******      **     **     **     **     **     **    **      *  \n");
	fprintf( stderr , " *      **      **   **      **     **     **     **     **   ***  \n");
	fprintf( stderr , " *       **      *****      ****   ****   ****   ****     ****  ***\n");
	fprintf( stderr , "**       * \n");
	fprintf( stderr , " **     ** \n");
	fprintf( stderr , "  ******   \n");
	sleep(1);
	fprintf( stderr , "   ***** **   ****      ****   ****      ****   \n");
	fprintf( stderr , "  **   ** *      *       **     **     **   **  \n");
	fprintf( stderr , " **     *        *        *     *     **     ** \n");
	fprintf( stderr , " **     *        *        **    *     **      * \n");
	fprintf( stderr , " **     *        *        **   *      *       * \n");
	fprintf( stderr , "  **   **        *         *   *      **********\n");
	fprintf( stderr , "  ******         *         **  *      *         \n");
	fprintf( stderr , " **              *         ** *       *         \n");
	fprintf( stderr , " **              *          ***       **        \n");
	fprintf( stderr , "  *******        *          ***       **      * \n");
	fprintf( stderr , " *      **       *          **         **    *  \n");
	fprintf( stderr , " *       **   *******        *           ****   \n");
	fprintf( stderr , "**       * \n");
	fprintf( stderr , " **     ** \n");
	fprintf( stderr , "  ******   \n");
	sleep(1);
	fprintf( stderr , "****   ****      *****      ***    *** \n");
	fprintf( stderr , "  *     *       **   **      **     ** \n");
	fprintf( stderr , "  *     *      **     **     **     ** \n");
	fprintf( stderr , "  **    *      *       *     **     ** \n");
	fprintf( stderr , "   *   *      **       **    **     ** \n");
	fprintf( stderr , "   **  *      **       **    **     ** \n");
	fprintf( stderr , "    *  *      **       **    **     ** \n");
	fprintf( stderr , "    * *       **       **    **     ** \n");
	fprintf( stderr , "    ***        *       *     **     ** \n");
	fprintf( stderr , "     *         **     **     **     ** \n");
	fprintf( stderr , "     *          **   **       *    ****\n");
	fprintf( stderr , "     *           *****        ***** ** \n");
	fprintf( stderr , "    * \n");
	fprintf( stderr , " ** * \n");
	fprintf( stderr , " ***  \n");
	sleep(1);
	fprintf( stderr , " ***     ***     ** ****   \n");
	fprintf( stderr , "  **      **    ****    ** \n");
	fprintf( stderr , "  **      **     **     ** \n");
	fprintf( stderr , "  **      **     **      **\n");
	fprintf( stderr , "  **      **     **      **\n");
	fprintf( stderr , "  **      **     **      **\n");
	fprintf( stderr , "  **      **     **      **\n");
	fprintf( stderr , "  **      **     **      **\n");
	fprintf( stderr , "  **      **     **      * \n");
	fprintf( stderr , "  **      **     **     ** \n");
	fprintf( stderr , "   *     ****    ***   **  \n");
	fprintf( stderr , "   ****** **     ** ****   \n");
	fprintf( stderr , "                 **        \n");
	fprintf( stderr , "                 **        \n");
	fprintf( stderr , "                *****      \n");
}
