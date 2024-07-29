#include <curl/curl.h>
#include <string.h>

#include "../include/seek.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int seek(const struct arguments *arguments, const struct wordlist *wordlist)
{
    if (arguments == NULL)
    {
        printf("Arguments are null\n");
        return STATUS_ERROR;
    }

    if (wordlist == NULL)
    {
        printf("Wordlist is null\n");
        return STATUS_ERROR;
    }

    FILE *fp = fopen("/dev/null", "r+");

    if (fp == NULL)
    {
        perror("Error on opening CURL output file");
        return STATUS_ERROR;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    CURL *handle = curl_easy_init();

    char *base_url = "http://127.0.0.1/";

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);

    for (int i = 0; i < wordlist->number_of_lines; i++)
    {
        char end_url[strlen(base_url) + strlen(wordlist->content[i])];

        memset(end_url, '\0', sizeof(end_url));

        strcat(end_url, base_url);
        strcat(end_url, wordlist->content[i]);

        curl_easy_setopt(handle, CURLOPT_URL, end_url);

        CURLcode response = curl_easy_perform(handle);

        if (response != CURLE_OK)
        {
            printf("CURL error: %s\n", curl_easy_strerror(response));

            return STATUS_ERROR;
        }

        long http_code = 0;

        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

        if (http_code < 400)
        {
            printf("Resource %s exists!\n", end_url);
        }
        else
        {
            printf("Resource %s does not exist!\n", end_url);
        }
    }

    curl_easy_cleanup(handle);

    curl_global_cleanup();
}