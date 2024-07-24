#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/file.h"
#include "../include/common.h"

#define MORE_LINES 1024
#define MORE_CHARS 1024

int file_content(const char *file_path, char **out_file_content)
{
    if (file_path == NULL)
    {
        printf("Null file path\n");
        return STATUS_ERROR;
    }

    FILE *fd = fopen(file_path, "r");

    if (fd == NULL)
    {
        perror("Error opening file");
        return STATUS_ERROR;
    }

    struct stat *stats = (struct stat *)malloc(sizeof(struct stat));

    if (stat(file_path, stats) == -1)
    {
        fclose(fd);
        free(stats);
        perror("Error getting stats for file");
        return STATUS_ERROR;
    }

    *out_file_content = (char *)malloc(stats->st_size - 1);

    char **file_content = malloc(sizeof(char *) * MORE_LINES);

    fclose(fd);
    free(stats);

    return STATUS_SUCCESS;
}