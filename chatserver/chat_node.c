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
	// create list
	ChatNodes* list = (ChatNodes*)malloc(sizeof(ChatNodes));

	// set head
	list->head = NULL;

	// set tail
	list->tail = NULL;

	return list;
}

// function to add chat node to structure
void addNode(ChatNodes* nodeList, ChatNode* newNode)
{

	/* chatnodelist structure,
	put chatnode data into list,
	add to end of list */
}

// function to remove a node
int removeNode(ChatNodes* nodeList, ChatNode* newNode)
{

	/* traverse node list to find target node,
	compare nodes, remove link from node,
	link previous node to node after target,
	free node mememory, return 0 if sccuessful */
	return -1;
}

// function for node comparison
/* compare all information in node structures,
	return true if all information matches or false
	if at least one returns false */
bool compareNodes(ChatNode* first, ChatNode* second)
{
	// compare if all node data is the same
	if (first->ip == second->ip && first->port == second->port &&
		strcmp(first->name, second->name) == 0)
	{
		// return true
		return true;
	}
	
	// else return false
	return false;
}