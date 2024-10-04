#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main()
{
    char filename[BUFFER_SIZE];

    if (fgets(filename, BUFFER_SIZE, stdin) == NULL)
    {
        perror("Failed to read filename");
        return 1;
    }

    filename[strcspn(filename, "\n")] = '\0';

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    char command[BUFFER_SIZE];
    if (fgets(command, BUFFER_SIZE, stdin) == NULL)
    {
        perror("Failed to read command");
        fclose(file);
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
        fprintf(file, "Error: Not enough numbers in command.\n");
        fclose(file);
        return 1;
    }

    int result = numbers[0];
    for (int i = 1; i < num_count; i++)
    {
        if (numbers[i] == 0)
        {
            fprintf(stderr, "Division by zero encountered. Terminating.\n");
            fprintf(file, "Error: Division by zero.\n");
            fclose(file);
            return 1;
        }
        result /= numbers[i];
    }

    fprintf(file, "Result: %d\n", result);

    fclose(file);

    printf("Result: %d\n", result);

    return 0;
}