// debug
#include "client_handler.h"

// function to talk with the client
/* create the socket and structure for receiving message,
	unlock the mutex, receive the message, switch case for message
	type */
void* talk_to_client(void* arg)
{
	// get the client socket from the arguments
	int clientSock = *((int*)arg);

	// structure for holding message
	Message clientMessage;

	// unlock the mutex for client socket
	pthread_mutex_unlock(&mutex_client_socket);

	// receive the message from client
	receiveMessage(clientSock, &clientMessage);

	// close the socket
	if (close(clientSock) == -1)
	{
		// throw a message for debugging
		debug("[CLIENT_HANDLER] Client handler failed to close client socket. Continuing...");
	}

	// convert 32 bit unsigned int ip from network byte order to host order
	clientMessage.chatNode.ip = ntohl(clientMessage.chatNode.ip);

	// convert 16 bit unsigned int port from network byte order to host order
	clientMessage.chatNode.port = ntohs(clientMessage.chatNode.port);

	// switch case for each message type
	switch(clientMessage.type)
	{
		// join message case
		case JOIN:
			{
				// call function for joining
				join(&clientMessage);

				// break
				break;
			}
		// leave message case
		case LEAVE:
			{
				// call function for leave
				leave(&clientMessage);

				// break
				break;
			}
		// note message case
		case NOTE:
			{
				// call function for note
				note(&clientMessage);

				// break
				break;
			}

		// shutdown all message case
		case SHUTDOWN_ALL:
			{
				// create temporary message for shutdown 
				Message* messageTerminate = messageNew(SHUTDOWN, NULL, NULL);

				// call function for shutting down all clients
				shutdownAll(messageTerminate);

				// server shutting down
				debug("[CLIENT_HANDLER] Serving is shutting down. Goodbye world.");

				// safely exit
				exit(EXIT_SUCCESS);
			}
	}
}

// join function
/* */
void join(Message* message)
{
	// variable declaration
	int socketChatNode;

	// structure for storing address info
	struct addrinfo hints, *server_info;

	// string for port
	char port[6];

	// varaible for error integer
	int errVar;

	// lock the mutex for node list
	pthread_mutex_lock(&mutex_chat_node_list);

	// let clients know that there is a new client joining
	message->type = JOIN;

	// get ready for traversal of the list
	ChatListElement* current = chatNodes->head;

	// traverse the list
	while(current != NULL)
	{
		// tell current client another client is joining
		debug("[CLIENT_HANDLER] Client joining. Standbye... ");

		// set bytes if hints structure to 0
		memset(&hints, 0, sizeof(hints));

		// create stream socket for TCP connection
		hints.ai_socktype = SOCK_STREAM;

		// return IPv4 addresses
		hints.ai_family = AF_INET;

		// print out the name of the node
		sprintf(port, "%u", current->chatNode.port);

		// get address info from the node and check for errors
		if ((errVar = getaddrinfo(ip_ntop(current->chatNode.ip), port, &hints, &server_info)) != 0)
		{
			// if error ecountered, throw debug statement
			debug("[CLIENT_HANDLER] Encountered error %s", gai_strerror(errVar));
		}

		// create socket for connection
		socketChatNode = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

		// establish the connection to the server
		if (connect(socketChatNode, server_info->ai_addr, server_info->ai_addrlen) != 0)
		{
			// throw an error
			perror("[CLIENT_HANDLER] Error connecting to the server.");
		}

		// send the message from server to client
		sendMessage(socketChatNode, message);

		// close the connection
		close(socketChatNode);

		// traverse to next chat node in list
		current = current->next;
	}

	// debug message notifying that the chat node joined
	debug("%s joined the chat.", message->chatNode.name);

	// unlock the mutex
	pthread_mutex_unlock(&mutex_chat_node_list);
}

// leave function
/* */
void leave(Message* message)
{
	// variable declaration
	int socketChatNode;

	// structure for storing address info
	struct addrinfo hints, *server_info;

	// string for port
	char port[6];

	// varaible for error integer
	int errVar;

	// lock the mutex for node list
	pthread_mutex_lock(&mutex_chat_node_list);

	// if the node to be found doesn't exist
	if (removeNode(chatNodes, &message->chatNode) == -1)
	{
		// tell client by throwing a debug
		debug("[CLIENT_HANDLER] Client node not found. Sorry!");
	}

	// or if found
	else
	{
		// notify other clients that a client has left
		message->type = LEAVE;

		// get ready for traversal of the list
		ChatListElement* current = chatNodes->head;

		// traverse the list
		while(current != NULL)
		{
			// tell current client that a client is leaving
			debug("[CLIENT_HANDLER] A client is leaving. Say bye!");

			// set bytes if hints structure to 0
			memset(&hints, 0, sizeof(hints));

			// create stream socket for TCP connection
			hints.ai_socktype = SOCK_STREAM;

			// return IPv4 addresses
			hints.ai_family = AF_INET;

			// print out the name of the node
			sprintf(port, "%u", current->chatNode.port);

			// get address info from the node and check for errors
			if ((errVar = getaddrinfo(ip_ntop(current->chatNode.ip), port, &hints, &server_info)) != 0)
			{
				// if error ecountered, throw debug statement
				debug("[CLIENT_HANDLER] Encountered error %s", gai_strerror(errVar));
			}

			// create socket for connection
			socketChatNode = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

			// establish the connection to the server
			if (connect(socketChatNode, server_info->ai_addr, server_info->ai_addrlen) != 0)
			{
				// throw an error
				perror("[CLIENT_HANDLER] Error connecting to the server.");
			}

			// send the message from server to client
			sendMessage(socketChatNode, message);

			// close the connection
			close(socketChatNode);

			// traverse to next chat node in list
			current = current->next;
		}

	// debug message notifying that the chat node left
	debug("%s left the chat.", message->chatNode.name);

	// unlock the mutex for node list
	pthread_mutex_unlock(&mutex_chat_node_list);
	}
}

// note function
/* */
void note(Message* message)
{
	// variable declaration
	int socketChatNode;

	// structure for storing address info
	struct addrinfo hints, *server_info;

	// string for port
	char port[6];

	// varaible for error integer
	int errVar;

	// lock the mutex for node list
	pthread_mutex_lock(&mutex_chat_node_list);

	// get ready for traversal of the list
	ChatListElement* current = chatNodes->head;

	// traverse the list
	while (current != NULL)
	{
		// if current node in list is the one who is sending the note
		if (compareNodes(&(current->chatNode), &(message->chatNode)))
		{
			// go to next node as to not send note to client
			current = current->next;

			// continue going through the list of nodes
			continue;
		}

		// debug statement for note being sent from client
		debug("[CLIENT_HANDLER] A note is being sent...");

		// set bytes if hints structure to 0
		memset(&hints, 0, sizeof(hints));

		// create stream socket for TCP connection
		hints.ai_socktype = SOCK_STREAM;

		// return IPv4 addresses
		hints.ai_family = AF_INET;

		// print out the name of the node
		sprintf(port, "%u", current->chatNode.port);

		// get address info from the node and check for errors
		if ((errVar = getaddrinfo(ip_ntop(current->chatNode.ip), port, &hints, &server_info)) != 0)
		{
			// if error ecountered, throw debug statement
			debug("[CLIENT_HANDLER] Encountered error %s", gai_strerror(errVar));
		}

		// create socket for connection
		socketChatNode = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

		// establish the connection to the server
		if (connect(socketChatNode, server_info->ai_addr, server_info->ai_addrlen) != 0)
		{
			// throw an error
			perror("[CLIENT_HANDLER] Error connecting to the server.");
		}

		// send the message from server to client
		sendMessage(socketChatNode, message);

		// close the connection
		close(socketChatNode);

		// traverse to next chat node in list
		current = current->next;
	}

	// debug message notifying that the chat node sent a note
	debug("%s sent a note.", message->chatNode.name);

	// unlock the mutex for node list
	pthread_mutex_unlock(&mutex_chat_node_list);
}

// shutdownall function
/* */
void shutdownAll(Message* message)
{
	// variable declaration
	int socketChatNode;

	// structure for storing address info
	struct addrinfo hints, *server_info;

	// string for port
	char port[6];

	// varaible for error integer
	int errVar;

	// lock the mutex for node list
	pthread_mutex_lock(&mutex_chat_node_list);

	// get ready for traversal of the list
	ChatListElement* current = chatNodes->head;

	// traverse the list
	while (current != NULL)
	{
		// tell current client another client is joining
		debug("[CLIENT_HANDLER] Server shutting down. Standbye... ");

		// set bytes if hints structure to 0
		memset(&hints, 0, sizeof(hints));

		// create stream socket for TCP connection
		hints.ai_socktype = SOCK_STREAM;

		// return IPv4 addresses
		hints.ai_family = AF_INET;

		// print out the name of the node
		sprintf(port, "%u", current->chatNode.port);

		// get address info from the node and check for errors
		if ((errVar = getaddrinfo(ip_ntop(current->chatNode.ip), port, &hints, &server_info)) != 0)
		{
			// if error ecountered, throw debug statement
			debug("[CLIENT_HANDLER] Encountered error %s", gai_strerror(errVar));
		}

		// create socket for connection
		socketChatNode = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

		// establish the connection to the server
		if (connect(socketChatNode, server_info->ai_addr, server_info->ai_addrlen) != 0)
		{
			// throw an error
			perror("[CLIENT_HANDLER] Error connecting to the server.");
		}

		// send the message from server to each client
		sendMessage(socketChatNode, message);

		// close the connection
		close(socketChatNode);

		// traverse to next chat node in list
		current = current->next;
	}

	// unlock the mutex for node list
	pthread_mutex_unlock(&mutex_chat_node_list);
}