// clientlist.cc
// Nathan Moon

// implementation of the functions in clientlist.h

#include "clientlist.h"

//pointer array to hold the client socket and list of client ids
Socket *theClients[MAX_CLIENTS];
char *theClientIds[MAX_CLIENTS];

int clientCount = 0;

/*return the number of clients in the client set*/
int numClients()
{
	return clientCount;
}

/*add client to the client list. Takes the client socket pointer.
  returns a bool indicating if client was successfully added*/
bool addClient(Socket *newClient)
{
	if (clientCount < MAX_CLIENTS) {
		//add client to set
		theClients[clientCount] = newClient;
		theClientIds[clientCount] = NULL;
		clientCount++;
		return true;
	}
	else
		return false;
}

//add client id to the client id list
bool addClientId(char clientId[], int fd)
{
	//copy clientId into temporary array
	char *id = strdup(clientId);
		
		//check if the client id is already contained in the client id list
		if (clientCount > 1) {
			for (int i = 0; i < clientCount; i++) {
				//do not check id if null
				if (theClientIds[i] != NULL) {
					if (strcmp(theClientIds[i], id) == 0){
						return false;
					}
				}
			}
		}
		//find find the appropriate index for the client id
		for (int i = 0; i < clientCount; i++) {
			if (theClients[i]->fd() == fd) {
				//add the id to the id array in the matching index
				theClientIds[i] = id;
				return true;
			}
		}
	
	return false;
}

/*remove client from client list. Takes an integer fd and returns
  a bool indicating if the client was successfully removed*/
bool removeClient(int fd)
{
	int socketIndex;
	bool socketFound;
	//find socket
	for (int i = 0; i < clientCount; i++) {
		if (theClients[i]->fd() == fd) {
			socketIndex = i;
			socketFound = true;
		}
	}
	if (socketFound) {
		//delete the target client socket
		theClients[socketIndex]->close();
		delete theClients[socketIndex];
		
		//replace deleted socket with last socket in client list
		theClients[socketIndex] = theClients[clientCount - 1];
		
		//replace client id with last client id
		if (theClientIds[socketIndex] != NULL) {
			//delete dynamic client id string
			char *id = theClientIds[socketIndex];
			delete id;
			//move last element to replace client id
			theClientIds[socketIndex] = theClientIds[clientCount - 1];
		}
		else
			theClientIds[socketIndex] = NULL;
		
		//decrement clientCount
		clientCount--;
		
		return true;
	}
	return false;
}

/*find the client in the client list. Takes the fd and returns the 
  client socket pointer*/
Socket *findClient(int fd)
{
	for (int i = 0; i < clientCount; i++) {
		//if given fd == current client fd, return current client
		if (fd == theClients[i]->fd())
			return theClients[i];
	}
	return NULL;
}

/* find the client id in the client id list. Takes the fd and returns the client id*/
char *findClientId(int fd)
{
	for (int i = 0; i < clientCount; i++) {
		if (fd == theClients[i]->fd())
			return theClientIds[i];
	}
	return NULL;
}

/*close and delete all of the client sockets in the client list*/
void closeAll()
{
	printf("\n"); //begin messages on new line
	for (int i = 0; i < clientCount; i++) {
		//close socket and delete socket object
		int portNum = theClients[i]->port();
		theClients[i]->close();
		delete theClients[i];
		delete theClientIds[i];
		printf("Server: client removed at port %d\n", portNum);
	}
	printf("Server: all clients removed\n");
}
	
/*return the client socket given an index position in the client list*/
Socket *getClient(int indexPosition)
{
	if (indexPosition < clientCount) {
		return theClients[indexPosition];
	}
	else //return null if the given index is out of bounds of the client list
		return NULL;
}

/*return the client id given an index position in the client list*/
char *getClientId(int indexPosition)
{
	if (indexPosition < clientCount) {
		return theClientIds[indexPosition];
	}
	else 
		return NULL;
}