#define DBG
#include "dbg.h"
#include "main.h"

// main function
int main(int argc, char* argv[])
{
	// create storage for client properties structure
	ClientProperties* properties;

	// get properties file for client
	char* propertiesFile = "default.properties";

	// if two arguments count is two
	if (argc == 2)
	{
		// file provided on command prompt, use it
		propertiesFile = argv[1];
	}

	// read properties file and store it
	properties = property_read_properties(propertiesFile);

	// create sender tread
	pthread_t senderThread;

	// if sender thread wasn't created
	if (pthread_create(&senderThread, NULL, send_to_server, (void*)properties) != 0)
	{
		// raise error to client
		debug("[MAIN_CLIENT] Sender thread failed to create. Exiting...");

		// exit safely
		exit(EXIT_FAILURE);
	}

	// create receiver thread
	pthread_t receiverThread;

	// if receiver thread wasn't created
	if (pthread_create(&receiverThread, NULL, receive_from_server, (void*)properties) != 0)
	{
		// raise error to client
		debug("[MAIN_CLIENT] Receiver thread failed to create. Exiting...");

		// exit safely
		exit(EXIT_FAILURE);
	}

	// join the receiver thread. if shutdown, exit safely
	if (pthread_join(receiverThread, NULL) != 0)
	{
		// 
		debug("[MAIN_CLIENT] Receiver thread failed to join. Exiting...");

		// exit safely
		exit(EXIT_FAILURE);
	}

	// exit safely
	return EXIT_SUCCESS;
}