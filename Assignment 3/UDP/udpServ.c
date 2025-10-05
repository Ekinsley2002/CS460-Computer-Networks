#include "udpServ.h"

int main()
{
	// buffer to hold data
	char buffer[250];

	// socket descriptor for server
	int server_socket;

	// storage for length of data received 
	ssize_t notion;

	// struct for naming the server and client address
	struct sockaddr_in servAddress, clientAddress;

	// get length of client address
	socklen_t length = sizeof(clientAddress);

	// client side socket
	int foreign_socket;

	// client socket naming struct
	struct addrinfo hints, *servinfo, *p;

	// address
	int rv;

	// variable to store data from nist server
	int data;

	// variable to find length of buffer
	int inLen = 0;

	// create UDP socket
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);

	// if socket creation fails
	if (server_socket < 0)
	{
		// notify user
		perror("Socket creation failed. Bye!");

		// exit safely
		exit(EXIT_FAILURE);
	}

	memset(&servAddress, 0, sizeof(servAddress));

	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddress.sin_port = htons(LOCAL_PORT);

	servAddress.sin_family = AF_INET;

	// bind server address to socket
	if (bind(server_socket, (struct sockaddr*)&servAddress, sizeof(servAddress)) < 0)
	{
		// if bind failed, raise error
		perror("Bind failed. Try again.");

		// close server
		close(server_socket);

		// safely exit
		exit(EXIT_FAILURE);
	}

	// get message from server
	notion = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, 
		(struct sockaddr*)&clientAddress, &length);

	// if data retrieval failed
	if (notion < 0)
	{
		// raise error
		perror("Data retrieval failed. Oops!\n");

		// close server
		close(server_socket);

		// safely exit
		exit(EXIT_FAILURE);
	}

	// intialize all of hints structure to 0
		// function: memset()
	memset(&hints, 0, sizeof hints);

	// use AF_INET6 to force IPv6
	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = SOCK_STREAM;

	// get the address information
		// function: getaddrinfo()
	rv = getaddrinfo(SERVER_ADDR, "13", &hints, &servinfo);

	if (rv != 0)
	{
		// if hostname and service failed
			// function: fprintf(), gai_strerror()
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

		// close the server
		close(server_socket);

		// safely exit
			// function: exit()
		exit(EXIT_FAILURE);
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

		// free address info
		freeaddrinfo(servinfo);

		// close server
		close(server_socket);

		// exit safely
			// function: exit()
		exit(EXIT_FAILURE);
	}

	// structure is completed
		// function: freeaddrinfo()
	freeaddrinfo(servinfo);

	// if there is data received
	while ((data = recv(foreign_socket, &buffer[inLen], 1, 0)) > 0)
	{
		// if the asterisk is found
		if (buffer[inLen] == '*')
		{
			// increment the length variable
			inLen++;

			// break out 
			break;
		}
		
		// keee incrementing until length is found
		inLen++;

		// if length of buffer is reached
		if (inLen >= sizeof(buffer) - 1)
		{
			// break out of loop
			break;
		}
	}

	// null terminate the end of the buffer
	buffer[inLen] = '\0';

	// close the socket from nist
	close(foreign_socket);

	// send response to client
	sendto(server_socket, buffer, strlen(buffer), 0, 
		(struct sockaddr*)&clientAddress, sizeof(clientAddress));

	// close the server
	close(server_socket);

	// return 0
	return 0;
}