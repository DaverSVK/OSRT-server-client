all: moj_server.o forkklient.o output
output: moj_server.o forkklient.o
	gcc moj_server.o -o moj_server -lpthread -lrt
	gcc forkklient.o -o forkklient
	rm *.o
	@xterm ./moj_server &
	@sleep 1
	@xterm ./forkklient &
	
	@xterm ./forkklient &
	
forkklient.o: forkklient.c
	gcc -c forkklient.c
moj_server.o: moj_server.c 
	gcc -c moj_server.c -lpthread
	
