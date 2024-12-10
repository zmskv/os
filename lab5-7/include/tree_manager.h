#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

typedef struct Node
{
    int id;
    int parent_id;
    int active;
    struct Node *left;
    struct Node *right;
} Node;

void initialize_tree();
Node *create_node(int id, int parent_id);
int add_node(int id, int parent_id);
Node *find_node(Node *root, int id);
void print_tree(Node *root);
void handle_create_command(const char *command);
void handle_exec_command(const char *command);
void handle_ping_command(const char *command);
void cleanup_tree(Node *root);

#endif
