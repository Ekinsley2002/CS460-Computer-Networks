#include "sender_handler.h"

void *send_to_server(void* arg) {
    
    // variable and function declarations
    int socket_to_server;
    struct addrinfo hints, *server_info;
    int error;

    char input_line[80];
    bool has_joined = false;

    Properties* properties = (Properties*) arg;

    char* server_port = property_get_property(properties, "SERVER_PORT");
    char* server_address = property_get_property(properties, "SERVER_ADDR");

    char* my_ip = property_get_property(properties, "MY_IP");
    char* my_port = propert_get_property(properties, "MY_PORT");
    char* my_name = property_get_property(properties, "MY_NAME");

    ChatNode* chat_node_myself = chat_node_new(htonl(ip_pton(my_ip)), htons(atoi(my_port)));

    // allocates memory and sets to zero for address info
    memset(&hints, 0, sizeof(hints));

    // Sets address info
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    if ((error = getaddrinfo(server_address, server_port, &hints, &server_info)) != 0)
    {
        debug("Error %d : %s\n", error, gai_strerror(error));
    }

    printf("Client up and running!");

    while(true)
    {
        // read the input line
        memset(input_line, '\0', sizeof(input_line));
        fgets(input_line, sizeof(input_line), stdin);

        if(starts_with(input_line, "JOIN"))
        {

            if(has_joined)
            {
                printf("You have already joined chat\n");
                continue;
            }

            Message* message_ptr = message_new(JOIN, chat_node_myself, NULL);

            socket_to_server = socket(server_info->ai_family, server_info->ai_socket);

            if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addr))
            {
                perror("[SENDER] Connection unable to be performed. Exiting...");
                exit(EXIT_FAILURE);
            }

            if (send_message(socket_to_server, message_ptr) == -1)
            {
                printf("[SENDER] There was an error trying to send message.");
            }

            has_joined = true;

            free(message_ptr);

            close(socket_to_server);
        }
    
        else if(starts_with(input_line, "LEAVE"))
        {
            if(!has_joined)
            {
                printf("You have not joined chat\n");
                continue;
            }

            Message* message_ptr = message_new(LEAVE, chat_node_myself, NULL);

            socket_to_server = socket(server_info->ai_family, server_info->ai_socket);

            if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addr))
            {
                perror("[SENDER] Connection unable to be performed. Exiting...");
                exit(EXIT_FAILURE);
            }

            if(send_message(socket_to_server, message_ptr) == -1)
            {
                printf("[SENDER] There was an error trying to send message.");
            }

            has_joined = false;

            free(message_ptr);

            close(socket_to_server);
        }

        else if (starts_with(input_line, "SHUTDOWN ALL") || starts_with(input_line, "SHUTDOWN"))
        {
            int type;

            if(starts_with(input_line, "SHUTDOWN ALL"))
            {
                if(!has_joined)
                {
                    printf("You have not joined chat\n");
                    continue;
                }
                type = SHUTDOWN_ALL;
            }
            else 
            {
                if(!has_joined)
                {
                    printf("Shutting down ...");
                    exit(EXIT_SUCCESS);
                }
            }

            type = LEAVE;
        }

        Message* message_ptr - message_new(type, chat_node_myself, NULL);

        socket_to_server = socket(server_info->ai_family, server_info->ai_socket);

        if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addr))
        {
            perror("[SENDER] Connection unable to be performed. Exiting...");
            exit(EXIT_FAILURE);
        }

        if(send_message(socket_to_server, message_ptr) == -1)
        {
            printf("[SENDER] There was an error trying to send message.");
        }

        free(message_ptr);

        debug("Shutting down");
        exit(EXIT_SUCCESS);
    }

    // Send a NOTE
    else
    {
        if(!has_joined)
        {
            printf("You have not joined chat\n");
            continue;
        }

        Message* message_ptr = message_new(NOTE, chat_node_myself, input_line);

        socket_to_server = socket(server_info->ai_family, server_info->ai_addr, server_info->ai_addr);

        if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addr))
        {
            perror("[SENDER] Connection unable to be performed. Exiting...");
            exit(EXIT_FAILURE);
        }

        if(send_message(socket_to_server, message_ptr) == -1)
        {
            printf("[SENDER] There was an error trying to send message.");
        }

        free(message_ptr);

        close(socket_to_server);
    }
}