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

    FILE *fd = fopen("/dev/null", "r+");

    if (fd == NULL)
    {
        perror("Error on opening CURL output file");
        return STATUS_ERROR;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    CURL *handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fd);

    const size_t base_url_length = strlen(arguments->url);

    for (int i = 0; i < wordlist->number_of_lines; i++)
    {
        char end_url[base_url_length + strlen(wordlist->content[i])];

        memset(end_url, '\0', sizeof(end_url));

        strcat(end_url, arguments->url);
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

        if (arguments->http_codes[http_code] == true)
        {
            printf("GET %s | HTTP Code: %ld\n", end_url, http_code);
        }
    }

    fclose(fd);

    curl_easy_cleanup(handle);

    curl_global_cleanup();
}