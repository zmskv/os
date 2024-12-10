#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/tree_manager.c"
#include "./src/rabbitmq_utils.c"

int main()
{
    char command[256];
    initialize_tree();
    rabbitmq_initialize();

    printf("Distributed System Command Line\n");
    while (1)
    {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "create", 6) == 0)
        {
            handle_create_command(command);
        }
        else if (strncmp(command, "exec", 4) == 0)
        {
            handle_exec_command(command);
        }
        else if (strncmp(command, "ping", 4) == 0)
        {
            handle_ping_command(command);
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strncmp(command, "kill", 4) == 0)
        {
            handle_kill_command(command);
        }
        else
        {
            printf("Error: Unknown command\n");
        }
    }

    cleanup_tree(NULL);
    rabbitmq_close();
    return 0;
}
