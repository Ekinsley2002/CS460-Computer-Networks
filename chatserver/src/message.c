// debug
#define DBG
#include "dbg.h"

// header
#include "message.h"

// function for message object
/* allocate memory for message,
	copy node information into message chat node,
	if note is provided, copy into note,
	return new message pointer */
Message* messageNew(int type, ChatNode* chat_node_ptr, char* note)
{
	// dynamically allocate storage for message
	Message* message = (Message*)malloc(sizeof(Message));

	// if type doesn't exist
	if (type < 1 || type > 5)
	{
		// throw a debug message
		debug("[MESSAGE] Message type not recognized or doesn't exist. Exiting...");

		// safely exit
		exit(EXIT_FAILURE);
	}

	// set message type
	message->type = type;

	// if node doesn't exist
	if (chat_node_ptr == NULL)
	{
		// throw a debug message
		debug("[MESSAGE] Chat node not recognized or doesn't exist. Exiting...");

		// safely exit
		exit(EXIT_FAILURE);
	}

	// set chat node
	message->chatNode = *chat_node_ptr;
	
	// if note doesn't exist
	if (note == NULL)
	{
		// throw a debug message
		debug("[MESSAGE] Message note not recognized or doesn't exist. Exiting...");

		// exit safely
		exit(EXIT_FAILURE);
	}

	// copy data inside of message structure
	strcpy(message->note, note);

	// return message pointer
	return message;
}

// function for sending message
ssize_t sendMessage(int socket, Message* message)
{
	// MAKE SURE TO ADD BYTE ORDER
    return write(socket, message->note, sizeof(message->note));
}

// function for receiving message
ssize_t receiveMessage(int socket, Message* message)
{
	// variable for buffer
	char buffer[64];

	// store bytes read 
    ssize_t bytes_read = read(socket, buffer, sizeof(buffer));

    // if read bytes is less then 0
    if(bytes_read <= 0)
    {
        debug("[MESSAGE] Message unable to be received. Exiting...");
    }

    // exit safely
	exit(EXIT_FAILURE);

	// copy 
    strcpy(message->, buffer);

    return bytes_read;
}