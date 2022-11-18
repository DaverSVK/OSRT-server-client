#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9502

int main(){
	
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[2000];
	char parbuffer[2000];
	pid_t childpid;
	int fd[2];
 
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	
		//prijmem prvu
		if(recv(clientSocket, buffer, 2000, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
		bzero(buffer, sizeof(buffer));
		if(recv(clientSocket, buffer, 2000, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}
		
		printf("Client: \t");
		scanf("%s", &buffer[0]);

		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		//bzero(buffer, sizeof(buffer));
		/*if(recv(clientSocket, buffer, 2000, 0) < 0){
			printf("[-]Error in receiving data.\n");}*/
		
		/*else{
			printf("Server: \t%s\n", buffer);
		}*/
		pipe(fd);
		if((childpid=fork())<0)
			printf("error\n");
		else if(childpid==0){
			
			int lenght=recv(clientSocket, buffer, 2000, 0);
			write(fd[1],buffer,lenght);
			close(fd[0]);
			
		}
		else{
			read(fd[0],parbuffer,2000);
			close(fd[1]);
			int i=0;
			while(parbuffer[i]!='\0')
			{
				printf("%c",parbuffer[i++]);
			}
			putchar('\n');
			//printf("Vasa sprava %s", parbuffer);
			//break;

		}
		getchar();
		getchar();

	return 0;
}