#include "udpServ.h"

int main()
{
	// buffer for storage
	char buffer[250];

	// variable for socket descriptor
	int sockfd;

	// structure for server addresses
	struct sockaddr_in servAddress, verifyADDR;

	// get length of verification
	socklen_t verifyLEN = sizeof(verifyADDR);

	// save a message for when the connection to server was accepted
	const char* message = "Server connection accpeted!";

	memset(&servAddress, 0, sizeof(servAddress));
	memset(&verifyADDR, 0, sizeof(verifyADDR));

	servAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddress.sin_port = htons(LOCAL_PORT);
	servAddress.sin_family = AF_INET;

	// socket creation for datagram
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// if socket creation failed
	if (sockfd < 0)
	{
		// raise an error
		perror("Socket creation failed. Sorry!");

		// safely exit
		exit(EXIT_FAILURE);
	}

	// request datagram package to be sent 
	sendto(sockfd, message, strlen(message), 0, 
		(struct sockaddr*)&servAddress, sizeof(servAddress));

	// wait for response from server
	ssize_t notion = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, 
		(struct sockaddr*)&verifyADDR, &verifyLEN);

	// if data retrieval failed
	if (notion < 0)
	{
		// raise an error
		perror("Data being received encountered an error. Try again next time.\n");

		// close the socket
		close(sockfd);

		// safely exit
		exit(EXIT_FAILURE);
	}

	// if the addresse of received packages doesn't match the original request
	if (verifyADDR.sin_addr.s_addr != servAddress.sin_addr.s_addr)
	{
		// raise an error
		printf("Verification failed. Try again.");

		// close socket
		close(sockfd);

		// safely exit
		exit(EXIT_FAILURE);
	}

	// null terminate end of buffer
	buffer[notion] = '\0';

	puts(buffer);

	// close socket descriptor
	close(sockfd);

	// return 0
	return 0;
}