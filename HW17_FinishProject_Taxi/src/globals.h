#ifndef GLOBALS_H
#define GLOBALS_H

#include <pwd.h>
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include "db_config.h"

#define STR_LEN 1000

extern PGconn			*g_conn;
extern char				*g_url;
extern AppIndicator		*g_indicator;
extern GtkWidget		*g_root;

#endif // GLOBALS_H
