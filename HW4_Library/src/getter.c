#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <curl/curl.h>

struct MemoryStruct
{
    char *memory;
    size_t size;
};

/**
 * @brief Функция обратного вызова
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return size_t
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp)
{
    // printf("%s\n", __FUNCTION__);
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

/**
 * @brief Выполнение GET запроса к заданному URL
 *
 * @param url
 * @return char* - результат выполнения запроса
 */
char *do_get(const char *url)
{
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
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

/**
 * @brief Разрешенные символы в описании местоположения
 */
static const char valid_characters[] = "@~+-.,";

/**
 * @brief Проверка строки местоположения
 *
 * В описании местоположения разрешены символы unicide,
 * английские быквы, цифры и символы из переменной valid_characters.
 *
 * Исходная строка обрезается при первом вхождении любого
 * неразрешенного символа
 *
 * @param buf  - исходная строка
 * @return int - количество символов в результирующей строке
 */
int sanitize_place(char *buf)
{
    bool valid_char;
    for (char *pc = buf; *pc != '\0'; pc++)
    {
        if ((unsigned char)*pc > 0x7f)
        {
            // *pc = '\0';
            // break;
            continue;
        }
        if (((*pc | 0x20) >= 'a') && ((*pc | 0x20) <= 'z'))
        {
            continue;
        }
        if ((*pc >= '0') && (*pc <= '9'))
        {
            continue;
        }
        valid_char = false;
        for (const char *vc = valid_characters; *vc != '\0'; vc++)
        {
            if (*pc == *vc)
            {
                valid_char = true;
                break;
            }
        }
        if (valid_char)
        {
            continue;
        }
        *pc = '\0';
        break;
    }
    return strlen(buf);
}

/**
 * @brief Создание URL запроса для заданного местоположения.
 *
 * После использования требуется освобождение памяти,
 * выделенной под строку URL/
 *
 * @param l      - заданное местоположение
 * @return char* - URL
 */
char *make_url(const char *l)
{
    char *out_buf = NULL;
    char *location;
    int location_len;

    location = (char *)malloc(strlen(l) + 1);
    if (location == NULL) {
        return NULL;
    }
    strcpy(location, l);
    location_len = sanitize_place(location);
    if (location_len == 0) {
        goto stop;
    }
    out_buf = (char *)malloc(location_len + 27);
    if (out_buf == NULL) {
        goto stop;
    }
    sprintf(out_buf, "https://wttr.in/%s?format=j1", location);

stop:
    free(location);
    return out_buf;
}
