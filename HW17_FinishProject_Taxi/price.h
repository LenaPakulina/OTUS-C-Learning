#ifndef PRICE_
#define PRICE_

#include <stdint.h>

typedef struct OptionInfo {
	int class_level;
	char* class_name;
	char* class_text;
	char* price_text;
	float waiting_time;
	float min_price;
	float price;
} OptionInfo_t;

typedef struct PriceInfo {
	char *currency;
	float distance;
	float time;
	OptionInfo_t *options;
	int size_options;
} PriceInfo_t;

//{
//  "currency": "RUB",
//  "distance": 61529.771101536542,
//  "options": [
//    {
//      "class_level": 50,
//      "class_name": "econom",
//      "class_text": "Эконом",
//      "min_price": 495,
//      "price": 10945,
//      "price_text": "10945 руб.",
//      "waiting_time": 203.98798614740372
//    }
//  ],
//  "time": 3816.9397069215775
//}

#endif PRICE_
