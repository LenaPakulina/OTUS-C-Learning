#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser_json.h"
#include "weather.h"
#include "getter.h"

int main(int argc, char* argv[])
{
    char *url;
    char *city = "Tumen";
    if (argc == 2) {
        city = argv[1];
    }

    url = make_url(city);
    if (url == NULL) {
        fprintf(stderr, "Incorrect response\n");
        return EXIT_FAILURE;
    }
    printf("Source URL: %s\n", url);

    char *resp = do_get(url);
    free(url);

    if (resp == NULL) {
        fprintf(stderr, "Error while do request to source URL\n");
        return EXIT_FAILURE;
    }

    int value = parse_weather_json(resp);
    free(resp);

    // Разбор и печать принятых данных
    if (value != 0) {
        printf("Failed to failed to parse json)\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
