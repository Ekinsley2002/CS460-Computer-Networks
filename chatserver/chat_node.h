#ifndef CHAT_NODE_H
#define CHAT_NODE_H

// libraries for node creations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// chat node structure
typedef struct chat_node_struct
{
	unsigned int ip;
	unsigned short int port;
	char name[16];
} ChatNode;

// structure for building linked list
typedef struct chat_node_list_element_struct
{
	ChatNode chat_node;
	struct chat_node_list_element_struct* next;
} ChatNodeListElement;

// chat node list structure
typedef struct chat_node_list_struct
{
	ChatNodeListElement* head;
	ChatNodeListElement* tail;
} ChatNodes;

// chat node function prototype
ChatNode* newNode(unsigned int ip, unsigned short int port, char* name);

// chat node list creation
ChatNodes* createChatList(void);

// adding and removing chat nodes
void addNode(ChatNodes* nodeList, ChatNode* newNode);
int removeNode(ChatNodes* nodeList, ChatNode* newNode);

// compare nodes
bool compareNodes(ChatNode* first, ChatNode* second);

#endif /* CHAT_NODE_H */