#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>
#include <stdarg.h>

#define con_pattern "host=localhost port=5432 user=postgres password=1234 dbname=%s"
#define select_pattern "SELECT %s(%s) FROM %s"

char glob_val[1024] = {0};

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

void getValue(PGconn * conn, const char *command,
			  const char *tableName, const char *columnNameStr)
{
	size_t select_len = sizeof (select_pattern) +
			strlen (command) + strlen (tableName) +
			strlen (columnNameStr) + 1;

	char *select_str = malloc(select_len);
	if (!select_str) {
		exit(EXIT_FAILURE);
	}

	snprintf (select_str, select_len, select_pattern, command, columnNameStr, tableName);
	printf("%s\n", select_str);

	PGresult *res = PQexec (conn, select_str);

	if (PQresultStatus (res) != PGRES_TUPLES_OK) {
		finish_with_error (conn);
	}

	int nrows = PQntuples (res);
	glob_val[0] = '\0';

	if (nrows) {
		char *val = PQgetvalue (res, 0, 0);
		memcpy (glob_val, val, sizeof (glob_val) - 1);
		if (strlen (val) <= sizeof (glob_val) - 1)
			glob_val[strlen (val)] = '\0';
		else
			glob_val[sizeof (glob_val) - 1] = '\0';
	}

	PQclear(res);
	free (select_str);
}

int main (int argc, char **argv)
{
	if (argc != 4) {
		print_error ("Use:app dataBaseName tableName columnNameStr.\n");
		return EXIT_FAILURE;
	}

	size_t con_len = sizeof (con_pattern) + strlen (argv[1]) + 1;
	char *con_str = malloc (con_len);
	snprintf (con_str, con_len, con_pattern, argv[1]);

	PGconn *conn = PQconnectdb (con_str);

	if (PQstatus (conn) != CONNECTION_OK) {
		finish_with_error (conn);
	}

	getValue(conn, "SUM", argv[2], argv[3]);
	printf ("Сумма: %s\n", glob_val);

	getValue(conn, "AVG", argv[2], argv[3]);
	printf ("среднее: %s\n", glob_val);

	getValue(conn, "MIN", argv[2], argv[3]);
	printf ("Минимум: %s\n", glob_val);

	getValue(conn, "MAX", argv[2], argv[3]);
	printf ("Максимум: %s\n", glob_val);

	getValue(conn, "var_samp", argv[2], argv[3]);
	printf ("Дисперсия: %s\n", glob_val);

	free (con_str);
	PQfinish(conn);

	/*
	 * SELECT SUM(age) FROM oscar
	   Сумма: 3215
	   SELECT AVG(age) FROM oscar
	   среднее: 36.1235955056179775
	   SELECT MIN(age) FROM oscar
	   Минимум: 21
	   SELECT MAX(age) FROM oscar
	   Максимум: 80
	   SELECT var_samp(age) FROM oscar
	   Дисперсия: 137.9504596527068437
	 *
	 */

	return EXIT_SUCCESS;

}
