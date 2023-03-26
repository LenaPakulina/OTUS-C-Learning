#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <time.h>

#include "parser_json.h"
#include "price.h"
#include "getter.h"
#include "db_integration.h"
#include "price.h"
#include "globals.h"

PGconn			*g_conn;
char			*g_url;
AppIndicator	*g_indicator;
GtkWidget		*g_root;

#define TRAY_APPINDICATOR_ID "runcat-applet"

/* timer callback, will change fps by cpu usage each time */
static gboolean tray_icon_update(gpointer data)
{
	char *resp = do_get(g_url);
	if (resp == NULL) {
		fprintf(stderr, "Error while do request to source URL\n");
		return EXIT_FAILURE;
	}

	PriceInfo_t price;
	int value1 = parse_taxi_price_json(&price, resp);
	free(resp);

	// Разбор и печать принятых данных
	if (value1 != 0) {
		printf("Failed to failed to parse json)\n");
		return EXIT_FAILURE;
	}

	insertPrice(g_conn, &price);

	if (price.options->price < 210) {
		app_indicator_set_icon(g_indicator, price1);
	} else {
		app_indicator_set_icon(g_indicator, price2);
	}

	for (int i = 0; i < price.size_options; i++) {
		free(price.options[i].class_name);
		free(price.options[i].class_text);
		free(price.options[i].price_text);
	}
	free(price.options);
	free(price.currency);

	g_timeout_add(2000, tray_icon_update, NULL);
	return 0;
}

int main(int argc, char* argv[])
{
	g_url = make_default_url();
	if (g_url == NULL) {
		fprintf(stderr, "Incorrect response\n");
		return EXIT_FAILURE;
	}

	g_conn = createPGconn();
	printf("Source URL: %s\n", g_url);

	srand(time(0));
	gtk_init(&argc, &argv);
	g_indicator = app_indicator_new(TRAY_APPINDICATOR_ID, price1,
								  APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
	app_indicator_set_status(g_indicator, APP_INDICATOR_STATUS_ACTIVE);
	g_root = gtk_menu_new();
	app_indicator_set_menu(g_indicator, GTK_MENU(g_root));
	g_timeout_add(2000, tray_icon_update, NULL);
	gtk_main();

	free(g_url);
	return EXIT_SUCCESS;
}
