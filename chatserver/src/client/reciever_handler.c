#include "reciever_handler.h"

pthread_mutex_t mutex_reciever_socket = PTHREAD_MUTEX_INITIALIZER;

void *receive_from_server(void* arg)
{
    // handles right after connection
    int receiver_socket;
    int client_socket;
    struct sockaddr_in server_address;
    int yes = 1;

    Properties* properties = (Properties*) arg;
    int my_port = atoi(property_get_property(properties, "MY_PORT"));

    signal(SIGPIPE, SIG_IGN);

    if ((receiver_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[listen_to_server] failed to create socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(receiver_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("[listen_to_server] failed to set socket options");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(my_port);

    if(bind(receiver_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0)
    {
        perror("[listen_to_server] failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if(listen(receiver_socket, NUM_CONNECTIONS) != 0)
    {
        perror("[listen_to_server] failed to listen on socket");
        exit(EXIT_FAILURE);
    }

    debug("[listen_to_server] receiver socket is listening on port");

    while(true)
    {
        pthread_mutex_lock(&mutex_reciever_socket);

        client_socket = accept(receiver_socket, NULL, NULL);
        debug(stderr,"connectd with server!");

        pthread_t thread;
        if (pthread_create(&thread, NULL, connection_to_server, (void*)&client_socket))
        {
            debug("[listen_to_server] error creating thread");
            exit(EXIT_FAILURE);
        }

        if(pthread_detach(thread))
        {
            debug("[listen_to_server] error detaching thread");
            exit(EXIT_FAILURE);
        }
    }
}

void *connection_to_server(void* arg)
{
    int client_socket = *((int*)arg);
    Message message;

    pthread_mutex_unlock(&mutex_reciever_socket);

    recieve_message(client_socket, &message);

    switch(message.type)
    {
        case NOTE:
        {
            printf("%s%s:%s %s", NOTE_COLOR, message.chat_node.name, RESET_COLOR, message.note);

            if( close(client_socket) == -1 )
            {
                perror("[connection_to_server] error closting socket");
            }
            else
            {
                debug('[connection_to_server] socket closed: %d', client_socket);
            }
            break;
        }
        case SHUTDOWN:
        {
            printf('Shutting down...\n');

            if (close(client_socket) == -1)
            {
                perror('[connection_to_server] error closing socket');
            }
            else
            {
                debug('[connection_to_server] socket closed: %d', client_socket);
            }

            exit(EXIT_SUCCESS);
        }
        case JOINING:
        {
            printf("%s%s joined the chat%s\n", JOINED_COLOR, message.chat_node.name, RESET_COLOR);

            if (close(client_socket) == -1)
            {
                perror('[connection_to_server] error closing socket');
            }
            else
            {
                debug('[connection_to_server] socket closed: %d', client_socket);
            }
            break;
        }
        case LEFT:
        {
            printf("%s%s left the chat%s\n", LEFT_COLOR, message.chat_node.name, RESET_COLOR);

            if (close(client_socket) == -1)
            {
                perror('[connection_to_server] error closing socket');
            }
            else
            {
                debug('[connection_to_server] socket closed: %d', client_socket);
            }
            break;
        }
    }
    pthread_exit(NULL);
}