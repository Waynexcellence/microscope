typedef struct Question{
	int type;
	char description[200];
	char option[4][50];
	int correct_answer;
}Question;
