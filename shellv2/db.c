#include "db.h"
#include <sqlite3.h> 
sqlite3 *db;

static int cb(void *NotUsed, int argc, char **argv, char **colName)
{
   for (size_t i =0; i< argc; i++)
	  printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
   
   printf("\n");
   return 0;
}

// int main(int argc, char* argv[])
// {
// 	sqlite3 *datab;
// 	char *errMessage;
// 	int rex;
// 	char *sql;

// 	rex = sqlite3_open("heartVENV.database", &datab);   //opening db
	
// 		sql = "CREATE TABLE SensorData("            //creating table
// 		 "TIME INT PRIMARY KEY     NOT NULL," 
// 		 "HEARTRATE           INT    NOT NULL," 
// 		 "ENV            INT     NOT NULL);";

// 	rex = sqlite3_exec(datab, sql, cb, 0, &errMessage);
// 	printf("%d", rex);

// }

void db_init()
{
	char *err;
	int rex = sqlite3_open("heartVENV.database", &db);
	char *sql = "CREATE TABLE SensorData("            //creating table
		 "TIME INT PRIMARY KEY     NOT NULL," \
		 "HEARTRATE           INT    NOT NULL," \
		 "ENV            INT     NOT NULL);";

	rex = sqlite3_exec(db, sql, cb, 0, &err);
	printf("%d", rex);
}