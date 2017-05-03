#include "db.h"

sqlite3 *db;
int pk;
bool usingDB = false;

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
		db_select();
	else if (strcmp(*(args + 1), "ins") == 0)
		db_insert("10:10:10", 33, "GREEN");
}

void db_insert(char *time, int rate, char *env)
{
	char sql[100];// = "";
				//"(0, 100, \"Hello\")";
	sprintf(sql, "INSERT INTO SensorData (TIME, RATE, ENV) VALUES (\"%s\", %d, \"%s\");", time, rate, env);

	printf("\n%s\n", sql);
	//strcat(sql, pk);
	//strcat(sql, ", ");
	//strcat(sql, rate);
	//strcat(sql, ", \"");
	//strcat(sql, env);
	// /strcat(sql, "\");");

	pk++;

	db_exec(sql);
}

char *db_select()
{
	char *sql = "SELECT * FROM SensorData";
	db_exec(sql);
	return NULL;
}

void db_init()
{

	char *sql = "CREATE TABLE SensorData"				\
				"("										\
					"PK INTEGER PRIMARY KEY AUTOINCREMENT,"	\
					"TIME TEXT NOT NULL,"				\
					"RATE INT NOT NULL,"				\
					"ENV  TEXT NOT NULL"				\
				");";

	db_exec(sql);
}

void db_exec(char *sql)
{
	while (usingDB)
		usleep(1000 * 1000);

	usingDB = true;

	int rex = sqlite3_open("processor.db", &db);
	char *err;

	rex = sqlite3_exec(db, sql, cb, 0, &err);

	printf("\nrex:%d\n", rex);
	printf("\nerr:%s\n", err);
	sqlite3_close(db);

	usingDB = false;
}