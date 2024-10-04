#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main()
{
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[0]);
        close(pipe2[1]);

        execl("./child", "child", (char *)NULL);
        perror("Execl failed");
        return 1;
    }
    else
    {
        close(pipe1[0]);
        close(pipe2[1]);

        char fileName[BUFFER_SIZE];
        printf("Enter filename: ");
        scanf("%s", fileName);

        write(pipe1[1], fileName, strlen(fileName));
        write(pipe1[1], "\n", 1);

        char command[BUFFER_SIZE];
        printf("Enter command (numbers separated by spaces): ");
        getchar();
        fgets(command, BUFFER_SIZE, stdin);

        write(pipe1[1], command, strlen(command));
        write(pipe1[1], "\n", 1);

        wait(NULL);

        close(pipe1[1]);

        char buffer[BUFFER_SIZE];
        read(pipe2[0], buffer, BUFFER_SIZE);
        printf("Child process output: %s\n", buffer);

        close(pipe2[0]);
    }

    return 0;
}