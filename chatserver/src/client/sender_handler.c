#include "sender_handler.h"

void *send_to_server(void* arg) {
    
    // variable and function declarations
    int socket_to_server;
    struct addrinfo hints, *server_info;
    int error;

    struct in_addr addr;

    char input_line[80];
    bool has_joined = false;

    Properties* properties = (Properties*) arg;

    char* server_port = property_get_property(properties, "SERVER_PORT");
    char* server_address = property_get_property(properties, "SERVER_ADDR");

    char* my_ip = property_get_property(properties, "MY_IP");
    char* my_port = property_get_property(properties, "MY_PORT");
    char* my_name = property_get_property(properties, "MY_NAME");

    inet_pton(AF_INET, my_ip, &addr);

    // Originally ip_pton but we used inet_pton instead
    ChatNode* chat_node_myself = newNode(addr.s_addr, htons(atoi(my_port)), my_name);

    // allocates memory and sets to zero for address info
    memset(&hints, 0, sizeof(hints));

    // Sets address info
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    if ((error = getaddrinfo(server_address, server_port, &hints, &server_info)) != 0)
    {
        debug("Error %d : %s\n", error, gai_strerror(error));
    }

    printf("Client up and running!\n");

    // while true
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

            Message* message_ptr = messageNew(JOIN, chat_node_myself, NULL);

            socket_to_server = socket(server_info->ai_family, server_info->ai_socktype, 0);

            if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addrlen))
            {
                perror("[SENDER] Connection unable to be performed. Exiting...");
                exit(EXIT_FAILURE);
            }

            if (sendMessage(socket_to_server, message_ptr) == -1)
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

            Message* message_ptr = messageNew(LEAVE, chat_node_myself, NULL);

            socket_to_server = socket(server_info->ai_family, server_info->ai_socktype, 0);

            if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addrlen))
            {
                perror("[SENDER] Connection unable to be performed. Exiting...");
                exit(EXIT_FAILURE);
            }

            if(sendMessage(socket_to_server, message_ptr) == -1)
            {
                printf("[SENDER] There was an error trying to send message.");
            }

            has_joined = false;

            free(message_ptr);

            close(socket_to_server);
        }

        else if (starts_with(input_line, "SHUTDOWN_ALL") || starts_with(input_line, "SHUTDOWN"))
        {
            int type;

            if(starts_with(input_line, "SHUTDOWN_ALL"))
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
                type = LEAVE;
            }
            
            Message* message_ptr = messageNew(type, chat_node_myself, NULL);

            socket_to_server = socket(server_info->ai_family, server_info->ai_socktype, 0);

            if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addrlen))
            {
                perror("[SENDER] Connection unable to be performed. Exiting...");
                exit(EXIT_FAILURE);
            }

            if(sendMessage(socket_to_server, message_ptr) == -1)
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

            Message* message_ptr = messageNew(NOTE, chat_node_myself, input_line);

            socket_to_server = socket(server_info->ai_family, server_info->ai_socktype, 0);

            if (connect(socket_to_server, server_info->ai_addr, server_info->ai_addrlen))
            {
                perror("[SENDER] Connection unable to be performed. Exiting...");
                exit(EXIT_FAILURE);
            }

            if(sendMessage(socket_to_server, message_ptr) == -1)
            {
                printf("[SENDER] There was an error trying to send message.");
            }

            free(message_ptr);

            close(socket_to_server);
        }
    }
}

bool starts_with(const char* str, const char* prefix) {
    if (str == NULL || prefix == NULL) {
        return false;
    }
    while (*prefix != '\0') {
        if (*str != *prefix) {
            return false;
        }
        str++;
        prefix++;
    }
    return true;
}