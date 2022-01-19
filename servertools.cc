// servertools.cc
// Nathan Moon

// Implementation of the functions in servertools.h

#include "servertools.h"

void send_response_200(char *pathname, Socket *client)
{
	char line1[] = "HTTP/1.1 200 OK\r\n";
	char line2[] = "Connection: close\r\n";
	//need to actually fetch time later
	char line3[50];
	fetch_time(line3);
	char line4[] = "Server: ServerGroup6 (Linux)\r\n";
	char line8[50];
	format_last_modified(pathname, line8);
	char line5[50];
	sprintf(line5, "Content-Length: %d\r\n", get_content_len(pathname));
	char line6[50];
	printf("pathname before: %s\n", pathname);
	get_content_type(pathname, line6);
	printf("pathname after: %s\n", pathname);
	char line7[] = "\r\n";
	
	
	printf("--SENT MESSAGE---------------\n");
	
	//send over the header
	client->send(line1, strlen(line1));
	printf("sent: %s\n", line1);
	client->send(line2, strlen(line2));
	printf("sent: %s\n", line2);
	client->send(line3, strlen(line3));
	printf("sent: %s\n", line3);
	client->send(line4, strlen(line4));
	printf("sent: %s\n", line4);
	client->send(line8, strlen(line8));
	printf("send: %s\n", line8);
	client->send(line5, strlen(line5));
	printf("sent: %s\n", line5);
	client->send(line6, strlen(line6));
	printf("sent: %s\n", line6);
	client->send(line7, strlen(line7));
	printf("sent return and newline\n");
	
	printf("-----------------------------\n");
	

	
	//send the document
	char buffer[8];
	int numRead;
	
	//open the file
	FILE *infile = fopen(pathname, "r");
	
	int count = 0;
	//priming read
	numRead = fread(buffer, 1, 1, infile);
	
	//loop until end of file reached
	while (!feof(infile)) {
		//send data block to client
		client->send(&buffer, numRead);
		//printf("sent new info %d\n", count);
		//modification read
		numRead = fread(buffer, 1, 1, infile);
		//send the last data block if 
		if (feof(infile)) {
			client->send(&buffer, numRead);
			//printf("sent new info %d\n", count);
		}
		count++;
	}
	
	
	fclose(infile);
	
}

void send_response_404(Socket *client)
{
	//define the header lines
	char line1[] = "HTTP/1.1 404 Not Found\r\n";
	char line2[] = "Connection: close\r\n";
	char line3[50];
	fetch_time(line3);
	char line4[] = "Server: ServerGroup6 (Linux)\r\n";
	char line5[] = "\r\n";
	
	//put header lines in array
	int headerArraySize = 5;
	char *headerArray[5] = {line1, line2, line3, line4, line5};
	
	//loop through the elements of the header array, sending each to client
	int count = 0;
	while (count < headerArraySize) {
		int result = client->send(headerArray[count], strlen(headerArray[count]));
		printf("result = %d", result);
		printf("head: %s", headerArray[count]);
		//increment count
		count++;
	}
	printf("made it here\n");
	
	//send a simple html document describing error
	char html1[] = "<html><head><title>Error 404: Not Found</title></head><body><h1>Error 404: Not Found</h1><p><b>The resource you requested could not be located.</b></p></body></html>";
	int size = strlen(html1); 
	
	//loop through sending all elements of html array
	//loop through the elements of the header array, sending each to client
	client->send(html1, size);

	
	
	printf("completed error message\n");
}

/* send a 400 bad request error to the client */
void send_response_400(Socket *client)
{
	//define the header lines
	char line1[] = "HTTP/1.1 400 Bad Request\r\n";
	char line2[] = "Connection: close\r\n";
	char line3[50];
	fetch_time(line3);
	char line4[] = "Server: ServerGroup6 (Linux)\r\n";
	char line5[] = "\r\n";
	
	//put header lines in array
	int headerArraySize = 5;
	char *headerArray[5] = {line1, line2, line3, line4, line5};
	
	//loop through the elements of the header array, sending each to client
	int count = 0;
	while (count < headerArraySize) {
		int result = client->send(headerArray[count], strlen(headerArray[count]));
		printf("result = %d", result);
		printf("head: %s", headerArray[count]);
		//increment count
		count++;
	}
	printf("made it here\n");
	
	//send a simple html document describing error
	char html1[] = "<html>";
	char html2[] = "<head>";
	char html3[] = "\t<title>Error 400: Bad Request</title>";
	char html4[] = "</head>";
	char html5[] = "<body>";
	char html6[] = "<h1>Error 400: Bad Request</h1>";
	char html7[] = "<p><b>Your browser sent an illegal request.</b></p>";
	char html8[] = "</body>";
	char html9[] = "</html>";
	
	//place all html lines in one array
	int htmlArraySize = 9;
	char *htmlArray[10] = {html1, html2, html3, html4, html5, html6, html7, html8, html9};
	
	
	//loop through sending all elements of html array
	//loop through the elements of the header array, sending each to client
	int htmlcount = 0;
	while (htmlcount < htmlArraySize) {
		client->send(htmlArray[htmlcount], strlen(htmlArray[htmlcount]));
		//increment count
		htmlcount++;
	}
	
	
	printf("completed error message\n");
}

/* send a 304 not modified response to the client */
void send_response_304(Socket *client)
{
	//define the header lines
	char line1[] = "HTTP/1.1 304 Not Modified\r\n";
	char line2[] = "Connection: close\r\n";
	char line3[50];
	fetch_time(line3);
	char line4[] = "Server: ServerGroup6 (Linux)\r\n";
	char line5[] = "\r\n";
	
	//put header lines in array
	int headerArraySize = 5;
	char *headerArray[5] = {line1, line2, line3, line4, line5};
	
	//loop through the elements of the header array, sending each to client
	int count = 0;
	while (count < headerArraySize) {
		int result = client->send(headerArray[count], strlen(headerArray[count]));
		printf("result = %d", result);
		printf("head: %s", headerArray[count]);
		//increment count
		count++;
	}
	printf("reached end good.");
}

void fetch_time(char *dateLine)
{
	//define time structs
	time_t t;
	struct tm *tmp;
	char formatted_time[60];
	
	//calculate current time
	time(&t);
	tmp = gmtime(&t);
	
	//format the time
	strftime(formatted_time, sizeof(formatted_time), "%a, %d %b %Y %T %Z", tmp);

	//create final string containing formatted time
	char final_time[60] = "Date: ";
	strcat(final_time, formatted_time);
	strcat(final_time, "\r\n");
	
	//copy the new date line into the given ptr
	strcpy(dateLine, final_time); 
}

// format a string containing the last modified header
void format_last_modified(char *pathname, char *lastModified)
{
	char stat_time[50];
	
	// gather the last modified time using stat func
	struct stat filestat;
	
	stat(pathname, &filestat);
	
	// store the time into a tm struct
	struct tm *tmp = gmtime(&filestat.st_mtime);
	
	// format the broken down time
	strftime(stat_time, 50, "%a, %d %b %Y %T %Z", tmp);
	
	char final_time[50] = "Last modified: ";
	strcat(final_time, stat_time);
	strcat(final_time, "\r\n");
	
	strcpy(lastModified, final_time);
}
	
	

/* gets the content length for the specified path name */
int get_content_len(char *path)
{
	//declare status struct
	struct stat filestatus;
	
	//call stat to get file status
	stat(path, &filestatus);
	
	//retrieve the size from filestatus and return
	printf("size %d\n", filestatus.st_size);
	return filestatus.st_size;
}

void get_content_type(char *path, char *headerptr)
{
	//create duplicate path
	char *pathdup = strdup(path); //create duplicate pathname for split func

	//split the path given the "/" delimiter
	char ** url = split(pathdup, '/', 10);
	
	//find the size of the split array
	int size = 0;
	while (url[size] != NULL) {
		size++;
	}
	
	//subtract one from size to find index of last url part
	int lastElement = size - 1;
	
	//split the last element (file name) based on a period char
	char ** filename = split(url[lastElement], '.', 5);
	
	//find the size of the split filename array
	int filenameSize = 0;
	while (filename[filenameSize] != NULL) {
		filenameSize++;
	}
	
	//file extension will be stored in array index size - 1
	int fileExtension = filenameSize - 1;
	
	//assign proper file type based on extension
	char fileType[12] = "";
	//make comparisons
	if (strcmp(filename[fileExtension], "html") == 0) {
		strcpy(fileType, "text/html");
	}
	else if (strcmp(filename[fileExtension], "gif") == 0) {
		strcpy(fileType, "image/gif");
	}
	else if (strcmp(filename[fileExtension], "jpeg") == 0) {
		strcpy(fileType, "image/jpeg");
	}
	else if (strcmp(filename[fileExtension], "jpg") == 0) {
		strcpy(fileType, "image/jpeg");
	}
	else {
		strcpy(fileType, "text/plain");
	}
	
	//create formatted header line for content type
	char formatType[30] = "Content-Type: ";
	strcat(formatType, fileType);
	strcat(formatType, "\r\n");
	
	//copy the formatted header into the given dest pointer
	strcpy(headerptr, formatType);
	
	//delete the split arrays
	deleteSplit(url);
	deleteSplit(filename);
}

char ** split(char *source, char delimit, int max)
{
  char **parts = new char *[max + 1];  // NULL is stored in the last element.
  
  int i = 0;
  char *token;
  
  token = strchr(source, delimit);
  while(token != NULL && i < max - 1) {
    *token = 0;    
    parts[i] = strdup(source);
    source = token + 1;
    token = strchr(source, delimit);
    i++;    
  }
  
  if( source && *source ) {
    parts[i] = strdup(source);
    i++;
  }
  
  parts[i] = NULL;   // used to indicate the end of the token list.
  return parts;
}


/* Deletes the structure used to store the string parts returned by the
   split() function. The pointer passed to the function should not be used
   after the function returns since the memory has been deallocated.
*/
void deleteSplit(char **parts)
{
  int i = 0;
  while(parts[i] != NULL) {
    delete parts[i];
    i++;
  }
  
  delete parts;
}

// send a bool back indicating whether the file at the pathname has been modified since the given time
bool was_modified(char *pathname, char *timestamp)
{
	// timestamp 
	printf("timestamp: [%s]\n", timestamp);
	
	// create time t type variables to hold the times
	time_t lastModified, requestTimestamp, result1, result2;										// seconds 
	
	// turn the given string into time
	
	// create tm struct to hold broken down given timestamp
	struct tm brokenTimestamp;	

	strptime(timestamp, " %a, %d %b %Y %T %Z", &brokenTimestamp);
	
	char string3[100];
	
	// place the time in calendar value - local time to calendar time representation
	requestTimestamp = mktime(&brokenTimestamp);

	// gather the last modified time using stat function
	struct stat filestat;
	stat(pathname, &filestat);
		
	// compare the times
	double diff = difftime(filestat.st_mtime, requestTimestamp);
	
	printf("time modified: %d\n", filestat.st_mtime);
	printf("time sent by browser: %d\n", requestTimestamp);
	printf("time difference: %d\n", diff); 
	if (diff > 0)
		return true;
	else
		return false;
	
}


	