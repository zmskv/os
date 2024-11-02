#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define SHM_NAME "/my_shared_memmory"

int main()
{
    int shm_fd;
    char *shm_ptr;

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("Failed to create shared memory");
        return 1;
    }

    if (ftruncate(shm_fd, BUFFER_SIZE) == -1)
    {
        perror("Failed to set the size of shared memory");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    shm_ptr = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("Memory mapping failed");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    char fileName[BUFFER_SIZE];
    printf("Enter the filename: ");
    scanf("%s", fileName);

    char command[BUFFER_SIZE];
    printf("Enter the command (numbers separated by spaces): ");
    getchar();
    fgets(command, BUFFER_SIZE, stdin);

    snprintf(shm_ptr, BUFFER_SIZE, "%s\n%s", fileName, command);

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Fork failed");
        munmap(shm_ptr, BUFFER_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    if (pid == 0)
    {
        execl("./child", "child", (char *)NULL);
        perror("execl failed");
        return 1;
    }
    else
    {
        wait(NULL);

        printf("Child process output: %s\n", shm_ptr);

        munmap(shm_ptr, BUFFER_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
