#include "db_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <stdbool.h>

bool hasStrValue(const config_t *config,
				 const char *path,
				 const char **value) {
	if (config_lookup_string(config, path, value)) {
		return true;
	}
	return false;
}

bool hasIntValue(const config_t *config,
				 const char *path, int *value) {
	if (config_lookup_int(config, path, value)) {
		return true;
	}
	return false;
}

bool hasFloatValue(const config_t *config,
				 const char *path, double *value) {
	if (config_lookup_float(config, path, value)) {
		return true;
	}
	return false;
}

int readConfigFile(char *path, DB_Config *dbInfo)
{
	config_t cfg;
	config_setting_t *setting;
	const char *str;

	config_init(&cfg); /* обязательная инициализация */

	/* Читаем файл. Если ошибка, то завершаем работу */
	if(! config_read_file(&cfg, path)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
				config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}

	bool hasErrors = false;
	hasErrors &= hasStrValue(&cfg, "hostName", &dbInfo->hostName);
	hasErrors &= hasStrValue(&cfg, "userName", &dbInfo->userName);
	hasErrors &= hasStrValue(&cfg, "password", &dbInfo->password);
	hasErrors &= hasStrValue(&cfg, "dbname", &dbInfo->dbname);
	hasErrors &= hasIntValue(&cfg, "portNum", &dbInfo->portNum);

	hasErrors &= hasStrValue(&cfg, "price1_Icon", &dbInfo->pathToPrice1);
	hasErrors &= hasStrValue(&cfg, "price2_Icon", &dbInfo->pathToPrice2);

	hasErrors &= hasStrValue(&cfg, "clid", &dbInfo->clid);
	hasErrors &= hasStrValue(&cfg, "apiKey", &dbInfo->apiKey);
	hasErrors &= hasStrValue(&cfg, "start_longitude", &dbInfo->start_longitude);
	hasErrors &= hasStrValue(&cfg, "start_latitude", &dbInfo->start_latitude);
	hasErrors &= hasStrValue(&cfg, "end_longitude", &dbInfo->end_longitude);
	hasErrors &= hasStrValue(&cfg, "end_latitude", &dbInfo->end_latitude);
	hasErrors &= hasStrValue(&cfg, "class_auto", &dbInfo->class_auto);
	hasErrors &= hasIntValue(&cfg, "acceptable_cost", &dbInfo->acceptable_cost);

	if(hasErrors) {
		printf("Errors occurred when reading the config");
		return -1;
	}

	return 0;
}
