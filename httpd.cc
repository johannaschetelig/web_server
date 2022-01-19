//httpd.cc
//Nathan Moon and Johanna Schetelig

/* This is a program which runs and manages a simple web server that recieves http
   requests and sends response messages containing the requested resource, or an 
   error message, as applicable */

#include "socket.h"
#include "fdset.h"
#include "clientlist.h"
#include "servertools.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

FDSet inputSet;
ServerSocket webserver;
const int PORT_NUMBER = 30018;

void ctrlcHandler(int sig);

//run the web server
int main(int argc, char *argv[])
{
	// signal handling
	signal(SIGINT, ctrlcHandler);
	
	
	//get port number from command line
	int port;
	
	//check number of args entered at command line
	if (argc < 2) {
		fprintf(stderr, "Error: Invalid number of arguments.\n");
		fprintf(stderr, "usage: server <portnum>\n\n");
		return( 1 );
	}
	else
		port = atoi(argv[1]);
	
	// bind the listening socket
	if (webserver.bind(port, 0) == 0){
		printf("\n-- Error binding to port --\n");
		closeAll();
		inputSet.clear();
		//close server socket
		webserver.close();
		exit(0);
	}
	printf("Server bound to port #%d\n", port);
	
	// add the listening socket to the FD set
	inputSet.addFD(webserver.fd());
	printf("added server to fd set, fd of %d\n", webserver.fd());
	
	// loop until the server is shut down
	while (true) {
		// wait for input on an input stream
		int *activeSet = inputSet.select();
				
		int i = 0;
		
		while (activeSet[i] >= 0) {
			int fd = activeSet[i];
			printf("the fd is %d\n", fd);
			
			// accept a connection
			if (fd == webserver.fd()) {
				Socket *client = webserver.accept();
				printf("dealing with client at %d\n", client->port());
				
				// add the client to client set
				// bool added = addClient(client);
				bool added = addClient(client); //take out later
				
				// add client to input set if added to client set
				if (added) {
					// add client to input set
					inputSet.addFD(client->fd());
					
					
					// print connection message
					printf("Server: accepted connection at port %d\n", client->port());
		
				}
				
				else {
					// send message saying bad request because max clients reached
					// close the client connection
					send_response_400(client);
				}
			}
			
			// process normal client input
			else {
				Socket *client = findClient(fd);
				printf("service client at port %d\n", client->port());
				// parse the http request sent by client
				
				// create char pointers to hold request header fields
				char requestLine[150];
				char connection[150];
				char *modifiedSince = 0;
				
				char curLine[200]; // char array to hold current request line
				int count = 0; // count to determine line number
				int result; //store how much data is recv over the socket
				printf("here2\n");
				// parse the request - priming read
				result = client->gets(curLine, 200);
				printf("---READING IN REQUEST-----------------\n");				
				while (curLine[0] != '\r' && result != 0) {
					
					char **splitLine = 0;
					
					// store in correct field according to field name
					printf("curline %s: ", curLine); 
					
					// store first line as request line
					if (count == 0) { 
						//requestLine = strdup(curLine); //delete after
						strcpy(requestLine, curLine);
					}
					
					// for all cases other than first line
					else {
						char colon = ':';
						// split the request on the colon
						splitLine = split(curLine, colon, 2);
						// store in connection field
						if (strcmp(splitLine[0], "Connection") == 0) {			
							strcpy(connection, splitLine[1]); 
						}
						
						// store in if modified since field
						else if (strcmp(splitLine[0], "If-Modified-Since") == 0) {
							//strcpy(modifiedSince, splitLine[1]);
							modifiedSince = strdup(splitLine[1]); 
							printf("modified since: %s\n", modifiedSince);
						}
						deleteSplit(splitLine);
					}
					// if result is 0, close the client socket
					
					// else, read next line and increment line count
					result = client->gets(curLine, 200);
					count++;
					printf("result was: %d\n", result); 
				}
				printf("---------------------------------\n");
				
				// determine appropriate message to send back
				// split the request line
				char **splitRequest = split(requestLine, ' ', 3);
				
				// send bad request
				// split the request line
				if (strcmp(splitRequest[0], "GET") != 0) {
					send_response_400(client);
					printf("sent bad request error to client\n");
				}
				
				// security check - send error 404 if browser tries to access parent directory
				else if (strstr(splitRequest[1], "../") != NULL) {
					send_response_404(client);
					printf("caught attempt to access parent directory\n");
					printf("sent error 404\n");
				}
				
				else {
				
					// format the pathname
					// prepend the 'www' root directory
					char finalPath[60] = "www";
					strcat(finalPath, splitRequest[1]);
					// append index.html if given path ends with '/'
					if (finalPath[strlen(finalPath) - 1] == '/') 
						strcat(finalPath, "index.html");
					
					// send response if file exists
					printf("\nfinal path: %s\n", finalPath);
					if (access(finalPath, R_OK) == 0) {
						printf("access to file granted\n");
						printf("modified since string HERE: '%s'\n", modifiedSince); 
						// send a 200 message request did not have conditional get field
						if (modifiedSince == 0) {
							send_response_200(finalPath, client);
							printf("200 response sent\n");
						}
						// request contained conditional get field
						else {
							/* determine if the file has been modified since timestamp
							   sent by the web browser */
							bool modifyStat = was_modified(finalPath, modifiedSince);
							
							// send error 304 if file was not modified
							if (!modifyStat) {
								printf("the file was modified\n");
								send_response_304(client);
							}
							
							// send 200 response if file was modified
							else {
								send_response_200(finalPath, client);
								printf("200 response sent\n");
							}
						}
					}
					
					// send error 404 if the file did not exist or could not be read from
					else 
						send_response_404(client);
				}
				
				// remove the client from inputSet and client list
				inputSet.removeFD(client->fd());
				removeClient(client->fd());
				printf("client removed\n");
			}
			i++;
		}
	}
}

//close the server cleanly
void ctrlcHandler(int sig) {
	//close any client connections
	closeAll();
	inputSet.clear();
	
	//close server socket
	webserver.close();
	printf("Server: server socket closed.\n");
	
	exit(0);
}
				
				
				
				
				
	