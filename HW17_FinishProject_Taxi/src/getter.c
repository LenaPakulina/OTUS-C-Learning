#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <curl/curl.h>
#include "db_config.h"

struct MemoryStruct
{
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
								  void *userp)
{
	printf("%s\n", __FUNCTION__);
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr)
	{
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

char *do_get(const char *url)
{
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;

	chunk.memory = NULL; /* will be grown as needed by the realloc above */
	chunk.size = 0;           /* no data at this point */

	printf("URL: %s\n", url);

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	res = curl_easy_perform(curl_handle);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
	}

	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();

	return chunk.memory;
}

char *make_default_url()
{
	char *clid = "ak230217";
	char *apiKey = "eoijZmtxulZlciAlFRkHYdUOwkWovgqohMRWMaEG";

	char *start_longitude = "56.797128";
	char *start_latitude = "60.580183";
	char *end_longitude = "56.836332";
	char *end_latitude= "60.605649";
	char *class1= "business";

	char *out_buf = NULL;
	out_buf = (char *)malloc(1000);
	if (out_buf == NULL) {
		goto stop;
	}

	sprintf(out_buf, "https://taxi-routeinfo.taxi.yandex.net/taxi_info?clid=%s&apikey=%s&rll=%s,%s~%s,%s&class=%s",
			g_config.clid, g_config.apiKey,
			g_config.start_latitude, g_config.start_longitude,
			g_config.end_latitude, g_config.end_longitude,
			g_config.class_auto);

stop:
	return out_buf;
}
