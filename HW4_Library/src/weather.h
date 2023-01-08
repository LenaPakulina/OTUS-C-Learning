#ifndef WEATHER_
#define WEATHER_

#include <stdint.h>

typedef struct AreaInfo {
    float feelsLikeC;
    float windspeedKmph;
    char* winddir16Point;
    char** weatherDescs;
} AreaInfo_t;

typedef struct WeartherInfo {
    AreaInfo_t *areas;
} WeartherInfo_t;

#endif
/*
"current_condition": [
    {
        "FeelsLikeC": "-24",
        "FeelsLikeF": "-12",
        "cloudcover": "75",
        "humidity": "77",
        "lang_ru": [
            {
                "value": "Небольшой снег"
            }
        ],
        "observation_time": "06:59 AM",
        "precipInches": "0.0",
        "precipMM": "0.0",
        "pressure": "1030",
        "pressureInches": "30",
        "temp_C": "-19",
        "temp_F": "-2",
        "uvIndex": "2",
        "visibility": "10",
        "visibilityMiles": "6",
        "weatherCode": "326",
        "weatherDesc": [
            {
                "value": "Light snow"
            }
        ],
        "weatherIconUrl": [
            {
                "value": ""
            }
        ],
        "winddir16Point": "W",
        "winddirDegree": "280",
        "windspeedKmph": "15",
        "windspeedMiles": "9"
    }
]
*/
