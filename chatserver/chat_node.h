#ifndef CHAT_NODE_H
#define CHAT_NODE_H

// libraries for node creations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// chat node structure
typedef struct ChatNode
{
	// variables for client information
	unsigned int ip;
	uint16_t port;
	char name[16];
} ChatNode;

// structure for building linked list
typedef struct ChatListElement
{
	ChatNode chat_node;
	struct ChatListElement* next;
} ChatListElement;

// chat node list structure
typedef struct ChatNodes
{
	ChatListElement* head;
	ChatListElement* tail;
} ChatNodes;

// chat node function prototype
ChatNode* newNode(unsigned int ip, unsigned short int port, char* name);

// chat node list creation
ChatNodes* createChatList(void);

// adding and removing chat nodes
void addNode(ChatNodes* nodeList, ChatNode* newNode);
int removeNode(ChatNodes* nodeList, ChatNode* targetNode);

// compare nodes
bool compareNodes(ChatNode* first, ChatNode* second);

#endif /* CHAT_NODE_H */