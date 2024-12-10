#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rabbitmq_utils.h"

#define HOSTNAME "localhost"
#define PORT 5672
#define QUEUE_NAME "task_queue"

amqp_connection_state_t conn;

void rabbitmq_initialize()
{
    conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket)
    {
        fprintf(stderr, "Error creating TCP socket\n");
        exit(1);
    }

    if (amqp_socket_open(socket, HOSTNAME, PORT))
    {
        fprintf(stderr, "Error opening TCP socket\n");
        exit(1);
    }

    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    amqp_channel_open(conn, 1);
    amqp_get_rpc_reply(conn);
}

void rabbitmq_send_message(const char *message)
{
    amqp_bytes_t queue = amqp_cstring_bytes(QUEUE_NAME);
    amqp_queue_declare(conn, 1, queue, 0, 0, 0, 1, amqp_empty_table);
    amqp_basic_publish(conn, 1, amqp_empty_bytes, queue, 0, 0, NULL, amqp_cstring_bytes(message));
}

void rabbitmq_close()
{
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}
