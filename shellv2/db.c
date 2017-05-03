#include "db.h"

sqlite3 *db;
int pk;

static int cb(void *NotUsed, int argc, char **argv, char **colName)
{
	for (size_t i =0; i< argc; i++)
		printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
   
	printf("\n");
	return 0;
}

void db_handler(char **args)
{
	if (*(args + 1) == NULL)
		return;

	if (strcmp(*(args + 1), "init") == 0)
		db_init();
	else if (strcmp(*(args + 1), "view") == 0)
		printf("\n%s\n", *(args + 1));
	else if (strcmp(*(args + 1), "ins") == 0)
		db_insert(33, "GREEN");
}

void db_insert(int rate, char *env)
{
	char sql[50];// = "";
				//"(0, 100, \"Hello\")";
	sprintf(sql, "INSERT INTO SensorData VALUES(%d, %d, \"%s\")", pk, rate, env);

	printf("\n%s\n", sql);
	//strcat(sql, pk);
	//strcat(sql, ", ");
	//strcat(sql, rate);
	//strcat(sql, ", \"");
	//strcat(sql, env);
	// /strcat(sql, "\");");

	pk++;

	//db_exec(sql);
}

char *db_select()
{
	return NULL;
}

void db_init()
{

	char *sql = "CREATE TABLE SensorData"				\
				"("										\
					"PK INT PRIMARY KEY NOT NULL,"		\
					"TIME TEXT NOT NULL,"				\
					"RATE INT NOT NULL,"				\
					"ENV  TEXT NOT NULL"				\
				");";

	db_exec(sql);
}

void db_exec(char *sql)
{
	int rex = sqlite3_open("processor.db", &db);
	char *err;

	rex = sqlite3_exec(db, sql, cb, 0, &err);

	printf("\nrex:%d\n", rex);
	printf("\nerr:%s\n", err);
	sqlite3_close(db);
}