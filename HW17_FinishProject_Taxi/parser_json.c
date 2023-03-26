#include "price.h"
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

void parse_int(int* destination, struct json_object* tokens, const char* name) {
	struct json_object* tmp = NULL;
	json_object_object_get_ex(tokens, name, &tmp);
	*destination = json_object_get_int(tmp);
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

int parse_taxi_price_json(PriceInfo_t *price, char* buffer) {
	printf("%s\n", buffer);

	struct json_object* parsed = json_tokener_parse(buffer);
	struct json_object* tokens = NULL;

	json_object_object_get_ex(parsed, "options", &tokens);
	price->size_options = json_object_array_length(tokens);

	price->options = NULL;
	price->options = calloc(sizeof(*price->options), price->size_options);
	if (price->options == NULL) {
		printf("Failed to allocate memory for name\n");
		json_object_put(parsed);
		return -1;
	}

	for (int i = 0; i < price->size_options; i++) {
		struct json_object* arr_item = json_object_array_get_idx(tokens, i);
		parse_int(&(price->options[i].class_level), arr_item, "class_level");
		parse_string(&(price->options[i].class_name), arr_item, "class_name");
		parse_string(&(price->options[i].class_text), arr_item, "class_text");
		parse_string(&(price->options[i].price_text), arr_item, "price_text");
		parse_float(&(price->options[i].min_price), arr_item, "min_price");
		parse_float(&(price->options[i].price), arr_item, "price");
		parse_float(&(price->options[i].waiting_time), arr_item, "waiting_time");
	}

	parse_string(&(price->currency), parsed, "currency");
	parse_float(&(price->distance), parsed, "distance");
	parse_float(&(price->time), parsed, "time");

	return 0;
}
