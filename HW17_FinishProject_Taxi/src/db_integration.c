#include "db_integration.h"
#include "price.h"
#include "globals.h"
#include "db_config.h"

#include <stdio.h>  // Для printf
#include <time.h>   // Для time, ctime

#define con_pattern "host=localhost port=5432 user=postgres password=1234 dbname=%s"
#define insert_pattern "INSERT into taxi_price(curr_time, price) values ('%s', %.0f)"

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

void insertPrice(PGconn * conn, PriceInfo_t *info)
{
	char currTimeFormat[STR_LEN] = { 0 };
	char insert_str[STR_LEN] = { 0 };

	// Переменная для сохранения текущего времени
	time_t mytime = time(NULL);
	struct tm *now = localtime(&mytime);

	snprintf(currTimeFormat, STR_LEN, "%d-%d-%d %d:%d:%d",
			  now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
			  now->tm_hour, now->tm_min, now->tm_sec);

	snprintf (insert_str, STR_LEN, insert_pattern, currTimeFormat, info->options[0].price);
	//printf("%s\n", insert_str);

	PGresult *res = PQexec (conn, insert_str);

	if (PQresultStatus (res) != PGRES_COMMAND_OK) {
		finish_with_error (conn);
	}
}

PGconn *createPGconn()
{
	char con_str[STR_LEN] = { 0 };
	snprintf (con_str, STR_LEN, con_pattern, g_config.dbname);

	PGconn *conn = PQconnectdb (con_str);
	if (PQstatus (conn) != CONNECTION_OK) {
		finish_with_error (conn);
	}

	return conn;
}
