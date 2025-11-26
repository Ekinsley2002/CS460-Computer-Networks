#ifndef MESSAGE_H
#define MESSAGE_H

// chat node library
#include "chat_node.h"

// sending message library
#include <unistd.h>

// global constant commands
#define JOIN 1
#define LEAVE 2
#define NOTE 3
#define SHUTDOWN 4
#define SHUTDOWN_ALL 5

// fixed length note
typedef char Note[64];

// network sent message
typedef struct Message
{
	unsigned char type;
	ChatNode chatNode;
	Note note;
} Message;

// function prototypes

// message functions

// create new message
Message* message_new(int type, ChatNode* chat_node_ptr, char* note);

// send message
ssize_t send_message(int socket, Message* message_ptr);

// receive message
ssize_t receive_message(int socket, Message* message_ptr);

#endif /* message.h */