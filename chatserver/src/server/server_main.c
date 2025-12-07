#define DBG 
#include "dbg.h"
#include "server_main.h"

// global variables
pthread_mutex_t mutex_client_socket = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_chat_node_list = PTHREAD_MUTEX_INITIALIZER;

// declare chat node linked list
ChatNodes* chatNodes;

// main function
int main(int argc, char* argv[])
{
	// variables for beginning initilization
	int serverSock;
	int clientSock;

	// structure for naming the server's socket for listening
	struct sockaddr_in servAddr;
	int yes = 1;

	// global nodes list for clients
	chatNodes = createChatList();

	// read properties from file
	char* propertiesFile = "default.properties";

	// if file is provided
	if (argc == 2)
	{
		// use it
		propertiesFile = argv[1];
	}

	// read the properties and store them
	Properties* properties = property_read_properties(propertiesFile);

	// ignore SIGPIPE
	signal(SIGPIPE, SIG_IGN);

	// create socket for server to begin listening
	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[SERVER_MAIN] Socket creation failed. Exiting...");
		exit(EXIT_FAILURE);
	}

	// get rid of address in use message
	if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		perror("[SERVER_MAIN] Setsockpt failed. Exiting...");
		exit(EXIT_FAILURE);
	}

	// bind the socket
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(property_get_property(properties, "SERVER_PORT")));

	// bind socket to server port if possible
	if (bind(serverSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) != 0)
	{
		// if failed, raise error
		perror("[SERVER_MAIN] Failed to bind to port. Exiting...");
		exit(EXIT_FAILURE);
	}

	// listen on the created socket
	if (listen(serverSock, NUM_CONNECTIONS) != 0)
	{
		// raise error if unable to listen
		perror("[SERVER_MAIN] Failed to listen on created socket. Exiting...");
		exit(EXIT_FAILURE);
	}

	// notify port being listened on
	debug("Server listening on port: %d\n", PORT);

	// begin the server loop
	while (TRUE)
	{
		// lock the mutex
		pthread_mutex_lock(&mutex_client_socket);

		// connect to client
		clientSock = accept(serverSock, NULL, NULL);
		fprintf(stderr, "\nClient connected. Hooray!");

		// create thread for client request
		pthread_t thread;

		// if creationg failed
		if (pthread_create(&thread, NULL, talk_to_client, (void*)&clientSock))
		{
			// throw debug statement
			debug("There was an error creating thread on server with PID %d. Exiting...", getpid());
			exit(EXIT_FAILURE);
		}

		// detach the thread
		if (pthread_detach(thread))
		{
			// if unable, throw debug statement
			debug("There was an error detaching thread on server with PID %d. Exiting...", getpid());
			exit(EXIT_FAILURE);
		}
	}
}