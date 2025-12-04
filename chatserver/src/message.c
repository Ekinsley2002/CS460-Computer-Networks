#include "message.h"

// create new message
Message* message_new(int type, ChatNode* chat_node_ptr, char* note)
{
    Message *message = (Message*)malloc(sizeof(Message));

    if( type < 1 || type > 5 )
    {
        debug("[MESSAGE] Message type not recognized or doesn't exist. Exiting...");

        exit(EXIT_FAILURE);
    }

    message->type = type;

    if( chat_node_ptr == NULL )
    {
        debug("[MESSAGE] Chat node not recognized or doesn't exist. Exiting...");

        exit(EXIT_FAILURE);
    }

    message->chatNode = *chat_node_ptr;

    if( note == NULL )
    {
        debug("[MESSAGE] Message note not recognized or doesn't exist. Exiting...");

        exit(EXIT_FAILURE);
    }

    strcpy(message->note, note);

    return message;
}

// send message (Make sure socket is defined)
ssize_t send_message(int socket, Message* message_ptr)
{

    // MAKE SURE TO ADD BYTE ORDER

    return write(socket, message_ptr->note, sizeof(message_ptr->note));
}

// receive message
ssize_t receive_message(int socket, Message* message_ptr)
{
    char buffer[64];

    ssize_t bytes_read = read(socket, buffer, sizeof(buffer));

    if(bytes_read <= 0)
    {
        return bytes_read;
    }

    strcpy(messge_ptr->note, buffer);

    return bytes_read;
}