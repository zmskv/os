#ifndef RABBITMQ_UTILS_H
#define RABBITMQ_UTILS_H

void rabbitmq_initialize();
void rabbitmq_send_message(const char *message);
void rabbitmq_close();

#endif
