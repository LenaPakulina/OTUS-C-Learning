#ifndef DB_INTEGRATION_H
#define DB_INTEGRATION_H

#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>
#include <stdarg.h>

#include "price.h"

void print_error(const char *format, ...);
void insertPrice(PGconn * conn, const char *tableName, PriceInfo_t *info);
PGconn *createPGconn();

#endif // DB_INTEGRATION_H
