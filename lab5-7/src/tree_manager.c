#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree_manager.h"
#include "../include/rabbitmq_utils.h"

Node *root = NULL;

void initialize_tree()
{
    root = NULL;
}

Node *create_node(int id, int parent_id)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->id = id;
    new_node->parent_id = parent_id;
    new_node->active = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

int add_node(int id, int parent_id)
{
    if (!root)
    {
        if (parent_id != -1)
        {
            return -1;
        }
        root = create_node(id, parent_id);
        return 0;
    }

    Node *parent = find_node(root, parent_id);
    if (!parent || !parent->active)
    {
        return -1;
    }

    if (!parent->left)
    {
        parent->left = create_node(id, parent_id);
    }
    else if (!parent->right)
    {
        parent->right = create_node(id, parent_id);
    }
    else
    {
        return -1;
    }
    return 0;
}

Node *find_node(Node *root, int id)
{
    if (!root)
        return NULL;
    if (root->id == id)
        return root;

    Node *left_result = find_node(root->left, id);
    if (left_result)
        return left_result;

    return find_node(root->right, id);
}

void print_tree(Node *root)
{
    if (!root)
        return;
    printf("Node ID: %d, Parent ID: %d, Active: %d\n", root->id, root->parent_id, root->active);
    print_tree(root->left);
    print_tree(root->right);
}

void handle_create_command(const char *command)
{
    if (command == NULL)
    {
        printf("Error: command is NULL\n");
        return;
    }

    int id, parent_id;
    int parsed = sscanf(command, "create %d %d", &id, &parent_id);

    if (parsed != 2)
    {
        printf("Error: Invalid command format (parsed %d values)\n", parsed);
        return;
    }

    if (add_node(id, parent_id) == 0)
    {
        printf("Ok: Node %d created\n", id);

        char message[256];
        snprintf(message, sizeof(message), "CREATE %d %d", id, parent_id);
        rabbitmq_send_message(message);
    }
    else
    {
        printf("Error: Could not create node %d\n", id);
    }
}

void handle_exec_command(const char *command)
{
    int id, n, sum = 0;
    char *token;
    char *rest = strdup(command);

    token = strtok(rest, " ");
    token = strtok(NULL, " ");
    if (!token || sscanf(token, "%d", &id) != 1)
    {
        printf("Error: Invalid command format\n");
        free(rest);
        return;
    }

    Node *target = find_node(root, id);
    if (!target || !target->active)
    {
        printf("Error: %d: Not found\n", id);
        free(rest);
        return;
    }

    token = strtok(NULL, " ");
    if (!token || sscanf(token, "%d", &n) != 1 || n <= 0)
    {
        printf("Error: %d: Invalid input\n", id);
        free(rest);
        return;
    }

    for (int i = 0; i < n; ++i)
    {
        token = strtok(NULL, " ");
        if (!token)
        {
            printf("Error: %d: Insufficient numbers\n", id);
            free(rest);
            return;
        }
        int number;
        sscanf(token, "%d", &number);
        sum += number;
    }

    printf("Ok: %d: %d\n", id, sum);
    free(rest);
}

void handle_ping_command(const char *command)
{
    int id;
    if (sscanf(command, "ping %d", &id) != 1)
    {
        printf("Error: Invalid command format\n");
        return;
    }

    Node *target = find_node(root, id);
    if (!target)
    {
        printf("Error: %d: Not found\n", id);
        return;
    }

    printf("Ok: %d: %d\n", id, target->active);
}

void cleanup_tree(Node *root)
{
    if (!root)
        return;
    cleanup_tree(root->left);
    cleanup_tree(root->right);
    free(root);
}

// Функция для "убийства" узла и всех его потомков
void kill_node(Node *node)
{
    if (!node)
        return;

    node->active = 0; // Отметить узел как неактивный

    // Рекурсивно убиваем всех дочерних узлов
    kill_node(node->left);
    kill_node(node->right);
}

void handle_kill_command(const char *command)
{
    int id;
    if (sscanf(command, "kill %d", &id) != 1)
    {
        printf("Error: Invalid command format\n");
        return;
    }

    Node *target = find_node(root, id);
    if (!target)
    {
        printf("Error:%d: Not found\n", id);
        return;
    }

    kill_node(target);

    printf("Ok: Node %d and its descendants are killed\n", id);

    char message[256];
    snprintf(message, sizeof(message), "KILL %d", id);
    rabbitmq_send_message(message);
}
