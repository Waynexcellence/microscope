typedef enum Human_race{
	Bamboo , Common , Patient
}Human_race;

typedef enum Bacteria_race{
	Ustilago_esculenta_Henn , virus
}Bacteria_race;

typedef struct Status{
	char name[200];
	int HP;
	int ATK;
	int Defense;
	int money;
	Human_race human_race;
	Bacteria_race bacteria_race;
}Status;

