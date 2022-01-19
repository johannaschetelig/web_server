// servertools.h
// Nathan Moon

// a module designed to work with web servers

#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

/* sends a 200 OK response to the client,
   where path name is the resource to send to client */
void send_response_200(char *pathname, Socket *client);

/* sends a 404 Not Found response to the client */
void send_response_404(Socket *client);

/* sends a 400 Bad Request resposne to the client */
void send_response_400(Socket *client);

/* sends a 304 not modified response to the client */
void send_response_304(Socket *client);

/* formats and returns a string containing the date header line */
void fetch_time(char *dateLine);

/* gets the content length for the specified path name */
int get_content_len(char *path);

/* formats and returns a string containing the content type header line */
void get_content_type(char *path, char *headerptr);

/* split a string on a given delimiter. returns an array of split items */
char ** split(char *source, char delimit, int max);

/* delete the split items from the split array */ 
void deleteSplit(char **parts);

/* send a bool back indicating whether the file at the pathname has been 
modified since the given time */
bool was_modified(char *pathname, char *timestamp);

/* formats and returns a string containing the last modified date of a file 
   to be used as the last-modified header */
void format_last_modified(char *pathname, char *lastModified);


