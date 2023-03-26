#ifndef GLOBALS_H
#define GLOBALS_H

#include <pwd.h>
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

extern PGconn			*g_conn;
extern char				*g_url;
extern AppIndicator		*g_indicator;
extern GtkWidget		*g_root;

char *price1 = "/home/lena/PROG/C-Learning2/HW17_FinishProject_Taxi/icons/price1.png";
char *price2 = "/home/lena/PROG/C-Learning2/HW17_FinishProject_Taxi/icons/wait.png";

#endif // GLOBALS_H
