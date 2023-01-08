#include "weather.h"
#include "parser_json.h"
#include<json-c/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void parse_float(float* destination, struct json_object* tokens, const char* name) {
    struct json_object* tmp = NULL;
    json_object_object_get_ex(tokens, name, &tmp);
    *destination = json_object_get_double(tmp);
}

void parse_string(char** destination, struct json_object* tokens, const char* name) {
    struct json_object* tmp = NULL;
    json_object_object_get_ex(tokens, name, &tmp);
    const char* value = json_object_to_json_string(tmp);
    const unsigned value_lenght = strlen(value) + 1;
    *destination = NULL;
    *destination = calloc(sizeof(*destination), value_lenght);
    if (*destination == NULL) {
        printf("Failed to allocate memory for name\n");
    }
    memcpy(*destination, value, sizeof(*value) * value_lenght);
}

int parse_weather_json(char* buffer) {
    //printf("%s\n", buffer);

    WeartherInfo_t weather;
    struct json_object* parsed = json_tokener_parse(buffer);
    struct json_object* tokens = NULL;

    json_object_object_get_ex(parsed, "current_condition", &tokens);
    const int size_weathers = json_object_array_length(tokens);
    weather.areas = NULL;
    weather.areas = calloc(sizeof(*weather.areas), size_weathers);
    if (weather.areas == NULL) {
        printf("Failed to allocate memory for name\n");
        json_object_put(parsed);
        return -1;
    }
    for (int i = 0; i < size_weathers; i++) {
        struct json_object* arr_item = json_object_array_get_idx(tokens, i);
        parse_float(&(weather.areas[i].feelsLikeC), arr_item, "FeelsLikeC");
        parse_string(&(weather.areas[i].winddir16Point), arr_item, "winddir16Point");
        parse_float(&(weather.areas[i].windspeedKmph), arr_item, "windspeedKmph");

        printf("FeelsLikeC: %f\n"
               "Winddir16Point: %s\n"
               "WindspeedKmph: %f\n",
               weather.areas[i].feelsLikeC,
               weather.areas[i].winddir16Point,
               weather.areas[i].windspeedKmph);

        struct json_object* extTokens = NULL;
        json_object_object_get_ex(arr_item, "weatherDesc", &extTokens);
        const int size_descWeathers = json_object_array_length(extTokens);
        weather.areas[i].weatherDescs = NULL;
        weather.areas[i].weatherDescs = calloc(sizeof(*weather.areas[i].weatherDescs), size_weathers);
        for (int j = 0; j < size_descWeathers; j++) {
            struct json_object* itemDesc = json_object_array_get_idx(extTokens, j);
            parse_string(&(weather.areas[i].weatherDescs[j]), itemDesc, "value");

            printf("weatherDesc%i: %s\n",
                   (j+1),
                   weather.areas[i].weatherDescs[j]);
        }
        free(weather.areas[i].weatherDescs);
    }
    free(weather.areas);
    json_object_put(parsed);
    
    return 0;
}
