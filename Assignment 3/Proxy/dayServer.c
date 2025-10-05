#include "dayServer.h"

// create a global mutex to protect from race condition
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// main function for server
int main(int argc, char** argv)
{
	// store the descriptor of server socket
	int server_socket;

	// struct for naming the server's address
	struct addrinfo hints, *servinfo, *p;

	// store the address
	int rv;

	// yes variable for later
	int yes = 1;

	// intialize all of hints structure to 0
	memset(&hints, 0, sizeof hints);

	// use AP_INET6 to force IPv6
	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = SOCK_STREAM;

	// use local IP address 
	hints.ai_flags = AI_PASSIVE;

	// get the address info
	if ((rv = getaddrinfo(NULL, PORT_PERSONAL, &hints, &servinfo)) != 0)
	{
		// raise gai_strerror 
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

		// safely exit
		exit(EXIT_FAILURE);
	}

	// signal sends when client disconnects
	signal(SIGPIPE, SIG_IGN);

	// loop through results and bind to first able
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		// create the socket
		if ((server_socket = socket(p->ai_family, p->ai_socktype, 
			p->ai_protocol)) == -1)
		{
			// raise an error if failed
			perror("Error creating socket.");

			// continue
			continue;
		}

		// remove error message for address in use
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			// if failed, raise error
			perror("setsockopt");

			// safely exit
			exit(EXIT_FAILURE);
		}

		// bind the server socket
		if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1)
		{
			// close the socket
			close(server_socket);

			// raise an error if failed
			perror("Error binding socket.");

			// continue 
			continue;
		}

		// break
		break;
	}

	// if binding failed
	if (p == NULL)
	{
		// tell user 
		fprintf(stderr, "Binding failed. Exiting...\n");

		// safely exit
		exit(EXIT_FAILURE);
	}

	// structure done
	freeaddrinfo(servinfo);

	// listen for client connections with pending put into queue
	if (listen(server_socket, NUM_CONNECTIONS) != 0)
	{
		// if cannot hear socket, raise error
		perror("Error when listening on socket. Try again");

		// safely exit
		exit(EXIT_FAILURE);
	}

	// continue the server until interrupt
	while (TRUE)
	{
		// accept connection to client
		int client_socket = accept(server_socket, NULL, NULL);

		// create thread to handle client requests
		pthread_t thread;

		// allocate memory to avoid using similar location
		int* arg = malloc(sizeof(int));

		// set socket descriptor or personal space
		*arg = client_socket;

		// if thread creation fails
		if (pthread_create(&thread, NULL, handle_client, 
			(void*)arg) != 0)
		{
			// raise an error
			perror("Error creating thread. Exiting...");

			// safely exit
			exit(EXIT_FAILURE);
		}

		// attempt to detach thread 
		if (pthread_detach(thread) != 0)
		{
			// raise an error
			perror("Error deteaching thread. Exiting...");

			// safely exit
			exit(EXIT_FAILURE);
		}
	}
}

// handle the client
void* handle_client(void* arg)
{
	// socket connected to client
	int client_socket = *((int*)arg);

	// free the allocated memory
	free(arg);

	// buffer
	char buffer[100];

	// client side socket
	int foreign_socket;

	// client socket naming struct
	struct addrinfo hints, *servinfo, *p;

	// address
	int rv;
	
	// variable to store received data from server
	int data;

	// intialize all of hints structure to 0
		// function: memset()
	memset(&hints, 0, sizeof hints);

	// use AF_INET6 to force IPv6
	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = SOCK_STREAM;

	// get the address information
		// function: getaddrinfo()
	rv = getaddrinfo(SERVER_ADDR, PORT, &hints, &servinfo);

	if (rv != 0)
	{
		// if hostname and service failed
			// function: fprintf(), gai_strerror()
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

		// exit the client safely using pthread
		pthread_exit(NULL);
	}

	// loop through results until socket connection
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		// if socket creation failed
			// function: socket()
		if ((foreign_socket = socket(p->ai_family, p->ai_socktype, 
			p->ai_protocol)) == -1)
		{
			// return an error
			perror("Socket creation failed.");

			// continue
			continue;
		}

		// connect to the server
			// function: connect()
		if (connect(foreign_socket, p->ai_addr, p->ai_addrlen) == -1)
		{
			// return an error if failed
			perror("Socket connection failed.");

			// close the socket
				// function: close()
			close(foreign_socket);

			// continue 
			continue;
		}

		// successfully connected
		break;
	}

	// if p looped off the end of the list with no connection
	if (p == NULL)
	{
		// raise an error
			// function: perror()
		perror("No connecton established. Exiting...\n");

		// exit the client safely using pthread
		pthread_exit(NULL);
	}

	// structure is completed
		// function: freeaddrinfo()
	freeaddrinfo(servinfo);

	// if there is data received
	while ((data = recv(foreign_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
	{
		// null terminate the buffer
		buffer[data] = '\0';

		// lock the mutex
		pthread_mutex_lock(&mutex);

		// print each character
		for (int i = 0; i < data; i++)
		{
			// print out each character
			printf("%c", buffer[i]);
		}

		// unlock the mutex
		pthread_mutex_unlock(&mutex);

		// send data to server
		send(client_socket, buffer, data, 0);

		// if asterisk is reached
		if (buffer[data - 1] == '*')
		{
			// close all sockets
			close(client_socket);
			close(foreign_socket);

			// exit safely using pthread
			pthread_exit(NULL);
		}
	}
}