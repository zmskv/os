#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SHM_NAME "/my_shared_memory"

int main()
{
    int shm_fd;
    char *shm_ptr;

    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("Failed to open shared memory");
        return 1;
    }

    shm_ptr = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("Memory mapping failed");
        close(shm_fd);
        return 1;
    }

    char fileName[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    sscanf(shm_ptr, "%[^\n]\n%[^\n]", fileName, command);

    FILE *file = fopen(fileName, "w");
    if (file == NULL)
    {
        perror("Failed to open file");
        snprintf(shm_ptr, BUFFER_SIZE, "Error: Failed to open file.\n");
        munmap(shm_ptr, BUFFER_SIZE);
        close(shm_fd);
        return 1;
    }

    int numbers[BUFFER_SIZE], num_count = 0;
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        numbers[num_count++] = atoi(token);
        token = strtok(NULL, " ");
    }

    if (num_count < 2)
    {
        snprintf(shm_ptr, BUFFER_SIZE, "Error: Not enough numbers in command.\n");
        fprintf(file, "Error: Not enough numbers in command.\n");
        fclose(file);
        munmap(shm_ptr, BUFFER_SIZE);
        close(shm_fd);
        return 1;
    }

    int result = numbers[0];
    for (int i = 1; i < num_count; i++)
    {
        if (numbers[i] == 0)
        {
            snprintf(shm_ptr, BUFFER_SIZE, "Error: Division by zero.\n");
            fprintf(file, "Error: Division by zero.\n");
            fclose(file);
            munmap(shm_ptr, BUFFER_SIZE);
            close(shm_fd);
            return 1;
        }
        result /= numbers[i];
    }

    fprintf(file, "Result: %d\n", result);
    snprintf(shm_ptr, BUFFER_SIZE, "Result: %d\n", result);

    fclose(file);
    munmap(shm_ptr, BUFFER_SIZE);
    close(shm_fd);

    return 0;
}
