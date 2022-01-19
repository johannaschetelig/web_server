all:
	make httpd
	
httpd: httpd.o clientlist.o servertools.o ../tools/socket.o ../tools/fdset.o
	g++ -o httpd httpd.o clientlist.o servertools.o ../tools/socket.o ../tools/fdset.o
	
httpd.o :  httpd.cc
	g++ -c -I ../tools httpd.cc
	
clientlist.o : clientlist.cc clientlist.h
	g++ -c -I ../tools clientlist.cc
	
servertools.o: servertools.cc servertools.h
	g++ -c -I ../tools servertools.cc
	
clean :
	rm -f httpd.o