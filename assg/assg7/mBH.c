#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Missing search query.\n");
        return 1;
    }

    char *query = argv[1];

    // Opens the database
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called\n";

    rc = sqlite3_open("places.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    char *sql_template = "SELECT datetime(moz_places.last_visit_date/1000000,'unixepoch'), moz_places.url, moz_places.title, moz_places.visit_count FROM moz_places WHERE url LIKE '%%%s%%';";

    // Calculate the length of the modified SQL query
    size_t sql_len = strlen(sql_template) + strlen(query) + 1;
    char *sql = (char*)malloc(sql_len);

    // Format the SQL query with the search query
    snprintf(sql, sql_len, sql_template, query);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
