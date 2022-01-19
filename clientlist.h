// clientlist.h
// Nathan Moon

// a module designed to hold and manage a list of clients for client/server applications

#include "socket.h"
#include "fdset.h"
#include <stdio.h>
#include <string.h>

const int MAX_CLIENTS = 10;

/*return the number of clients in the client set*/
int numClients();

/*add client to the client list. Takes the client socket pointer.
  returns a bool indicating if client was successfully added*/
bool addClient(Socket *);

/*add the client id to the client list*/
bool addClientId(char clientId[], int fd);

/*remove client from client list. Takes an integer fd and returns
  a bool indicating if the client was successfully removed*/
bool removeClient(int fd);

/*find the client in the client list. Takes the fd and returns the 
  client socket pointer*/
Socket * findClient(int fd);

/*find the client id in the client list. Takes the fd and return the string containing
  the client id*/
char *findClientId(int fd);

/*close and delete all of the client sockets in the client list*/
void closeAll();

/*find and return the client socket given an index position in the client list*/
Socket *getClient(int indexPosition);

/*find and return the client id given an index position in the client list*/
char *getClientId(int indexPosition);
