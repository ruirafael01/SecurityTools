#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/file.h"
#include "../include/common.h"

int file_content(const char *file_path, char **out_file_content)
{
    if (file_path == NULL)
    {
        printf("Null file path\n");
        return STATUS_ERROR;
    }

    int fd = open(file_path, O_RDONLY);

    if (fd == -1)
    {
        perror("Error opening file");
        return STATUS_ERROR;
    }

    struct stat *stats = (struct stat *)malloc(sizeof(struct stat));

    if (stat(file_path, stats) == -1)
    {
        close(fd);
        free(stats);
        perror("Error getting stats for file");
        return STATUS_ERROR;
    }

    *out_file_content = (char *)malloc(stats->st_size);

    read(fd, *out_file_content, stats->st_size);

    close(fd);
    free(stats);

    return STATUS_SUCCESS;
}