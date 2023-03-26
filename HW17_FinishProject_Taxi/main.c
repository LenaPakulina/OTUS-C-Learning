#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "parser_json.h"
#include "price.h"
#include "getter.h"
#include "db_integration.h"
#include "price.h"

void updateInfo(int signum) {
	fprintf(stderr, "UPDATE INFO ...\n");
	alarm(5);
}

int main(int argc, char* argv[])
{
	char *url;
	url = make_default_url();
	if (url == NULL) {
		fprintf(stderr, "Incorrect response\n");
		return EXIT_FAILURE;
	}

	printf("Source URL: %s\n", url);

	PGconn *conn = createPGconn();
	const char *tableName = "taxi_price";

	if (signal(SIGALRM, updateInfo) == SIG_ERR) {
		return EXIT_FAILURE;
	}

	alarm(5);

	while (1) {
		char *resp = do_get(url);
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

		insertPrice(conn, tableName, &price);

		for (int i = 0; i < price.size_options; i++) {
			free(price.options[i].class_name);
			free(price.options[i].class_text);
			free(price.options[i].price_text);
		}
		free(price.options);
		free(price.currency);

		pause();
	}

	free(url);

	return EXIT_SUCCESS;
}
