#include "db_integration.h"
#include "price.h"

#include <stdio.h>  // Для printf
#include <time.h>   // Для time, ctime

#define con_pattern "host=localhost port=5432 user=postgres password=1234 dbname=%s"
#define insert_pattern "INSERT into %s(curr_time, price) values ('%s', %f)"

void print_error(const char *format, ...)
{
	va_list argptr;
	va_start (argptr, format);
	vprintf (format, argptr);
	va_end (argptr);
	exit (EXIT_FAILURE);
}

static void __attribute__((noreturn)) finish_with_error (PGconn * conn)
{
	puts (PQerrorMessage (conn));
	PQfinish (conn);
	exit (EXIT_FAILURE);
}

void insertPrice(PGconn * conn, const char *tableName, PriceInfo_t *info)
{
	char *insert_str = malloc(300);
	if (!insert_str) {
		exit(EXIT_FAILURE);
	}

	// Переменная для сохранения текущего времени
	time_t mytime = time(NULL);
	struct tm *now = localtime(&mytime);
	char *currTimeFormat = malloc(100);
	if (!currTimeFormat) {
		exit(EXIT_FAILURE);
	}
	snprintf (currTimeFormat, 100, "%d-%d-%d %d:%d:%d",
			  now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
			  now->tm_hour, now->tm_min, now->tm_sec);

	snprintf (insert_str, 300, insert_pattern, tableName, currTimeFormat, info->options[0].price);
	printf("%s\n", insert_str);

	PGresult *res = PQexec (conn, insert_str);

	if (PQresultStatus (res) != PGRES_COMMAND_OK) {
		finish_with_error (conn);
	}

	free (insert_str);
}

PGconn *createPGconn()
{
	char *dataBaseName = "taxi";
	char *tableName = "taxi";
	char *columnNameStr = "taxi";

	size_t con_len = sizeof (con_pattern) + strlen (dataBaseName) + 1;
	char *con_str = malloc (con_len);
	snprintf (con_str, con_len, con_pattern, dataBaseName);

	PGconn *conn = PQconnectdb (con_str);

	if (PQstatus (conn) != CONNECTION_OK) {
		finish_with_error (conn);
	}

	return conn;
}

