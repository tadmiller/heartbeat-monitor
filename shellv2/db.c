#include "db.h"

sqlite3 *db;
bool usingDB = false;
bool updated = false;
int size = 0;

// For each column this is called
static int cb(void *notUsed, int argc, char **argv, char **colName)
{
	if (strcmp(colName[0], "MAX(pk)") == 0)
	{
		size = atoi(argv[0]);
		return 0;
	}
	
	for (size_t i = 0; i < argc; i++)
		printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
   
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
	else if (strcmp(*(args + 1), "size") == 0)
		printf("\nSIZE IS: %d\n", db_size());
}

void db_insert(char *time, int rate, char *env)
{
	char sql[100];
	sprintf(sql, "INSERT INTO SensorData (TIME, RATE, ENV) VALUES (\"%s\", %d, \"%s\");", time, rate, env);

	db_exec(sql, 'I');
}

void waitForCallback()
{
	while (!updated)
		usleep(100 * 1000);

	updated = false;

	return;
}

int db_size()
{
	char *sql = "SELECT MAX(pk) FROM SensorData";
	db_exec(sql, 'S');

	waitForCallback();

	return size;
}

char *db_select()
{
	char *sql = "SELECT * FROM SensorData";
	db_exec(sql, 'S');
	return NULL;
}

void db_init()
{

	char *sql = "CREATE TABLE SensorData"					\
				"("											\
					"PK INTEGER PRIMARY KEY AUTOINCREMENT,"	\
					"TIME TEXT NOT NULL,"					\
					"RATE INT NOT NULL,"					\
					"ENV  TEXT NOT NULL"					\
				");";

	db_exec(sql, 'C');
}

void db_exec(char *sql, char type)
{
	while (usingDB)
		usleep(1000 * 1000);

	usingDB = true;

	int rex = sqlite3_open("processor.db", &db);
	char *err;

	if (type == 'S')
		rex = sqlite3_exec(db, sql, cb, 0, &err);
	else
		rex = sqlite3_exec(db, sql, NULL, 0, &err);

	if (rex != 0)
		printf("\n%s\n", err);

	sqlite3_close(db);

	updated = true;
	usingDB = false;
}