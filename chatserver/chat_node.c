// debug
#define DBG
#include "dbg.h"

// header
#include "chat_node.h"

// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function for node creation
/* Create the new node,
	store ip, port, and name in structure,
	make sure info is copied correctly,
	return created chatnode */
ChatNode* newNode(unsigned int ip, unsigned short int port, char* name)
{
	// create the new node
	ChatNode* newNode;

	// store the ip inside the node
	newNode->ip = ip;

	// store the port inside the node
	newNode->port = port;

	// copy the name inside of the node
	strcpy(newNode->name, name);

	// return node
	return newNode;
}

// chat node list creation
/* create list for chat nodes,
	set head and tail pointers,
	return the new list */
ChatNodes* createChatList(void)
{
	// create dynamically allocated list
	ChatNodes* list = (ChatNodes*)malloc(sizeof(ChatNodes));

	// set head
	list->head = NULL;

	// set tail
	list->tail = NULL;

	// return list
	return list;
}

// function to add chat node to structure
/* chatnodelist structure,
	allocate data, put chatnode data into list,
	add to end of list */
void addNode(ChatNodes* nodeList, ChatNode* newNode)
{
	// allocate space for the node to be added to the list
	ChatListElement* element = (ChatListElement*)malloc(sizeof(ChatListElement));

	// if allocation fails
	if (element == NULL)
	{
		// notify user of failure
		debug("[NODE] List element was unable to be created. Exiting...");

		// exit safely
		exit(EXIT_FAILURE);
	}

	// add the node info into the list element
	element->chat_node = *newNode;

	// set next node after to be null
	element->next = NULL;

	// if list is empty
	if (nodeList->head == NULL)
	{
		// add node as head
		nodeList->head = element;

		// and as the tail
		nodeList->tail = element;

		// return
		return;
	}

	// or if not empty, add node to end of list
	nodeList->tail->next = element;
	nodeList->tail = element;
}

// function to remove a node
/* traverse node list to find target node,
	compare nodes, remove link from node,
	link previous node to node after target,
	free node mememory, return 0 if sccuessful */
int removeNode(ChatNodes* nodeList, ChatNode* targetNode)
{
	// set a traversal node
	ChatListElement* current = nodeList->head;

	while(current != NULL)
	{
		if (compareNodes(&current->chat_node, targetNode) == 1)
		{
			
		}

		current = current->next;
	}
	
	return -1;
}

// function for node comparison
/* compare all information in node structures,
	return true if all information matches or false
	if at least one returns false */
bool compareNodes(ChatNode* current, ChatNode* target)
{
	// compare if all node data is the same
	if (current->ip == target->ip && current->port == target->port &&
		strcmp(current->name, target->name) == 0)
	{
		// return true
		return true;
	}
	
	// else return false
	return false;
}