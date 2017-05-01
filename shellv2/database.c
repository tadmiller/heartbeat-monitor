#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

static int cb(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
    sqlite3 *datab;
    char *errMessage;
    int rex;
    char *sql;

    rex = sqlite3_open("heartVENV.database", &datab);   //opening db
    
        sql = "CREATE TABLE SensorData("  \ //creating table
         "TIME INT PRIMARY KEY     NOT NULL," \
         "HEARTRATE           INT    NOT NULL," \
         "ENV            INT     NOT NULL);";

    rex = sqlite3_exec(datab, sql, cb, 0, &zErrMsg);

}





