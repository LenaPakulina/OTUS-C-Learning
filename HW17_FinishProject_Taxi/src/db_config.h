#ifndef DB_SETTINGS_H
#define DB_SETTINGS_H

typedef struct DB_Config {
	char *hostName;
	char *userName;
	char *password;
	char *dbname;
	int portNum;

	char *pathToPrice1;
	char *pathToPrice2;

	char *clid;
	char *apiKey;

	char *start_longitude;
	char *start_latitude;
	char *end_longitude;
	char *end_latitude;
	char *class_auto;
	int acceptable_cost;
} DB_Config;

int readConfigFile(char *path, DB_Config *dbInfo);
extern DB_Config		g_config;

#endif // DB_SETTINGS_H
