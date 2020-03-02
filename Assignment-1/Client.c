#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <signal.h>

void * reception(void * sockID){
		int clientSocket = *((int *) sockID);
		while(1){
		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
		printf("%s\n",data);

	}

}


void catch(int dummy)
	{
     	char  c;
     	signal(dummy, SIG_IGN);
     	printf("EXIT? [y/n] ");
     	c = getchar();
     	if (c == 'y' || c == 'Y')
        	exit(0);
     	else
        	signal(SIGINT, catch);
     	getchar();
	}

int main(int argc,char *argv[]){

	int port;
	char username[100];
	
	port = atoi(argv[2]);
	strcpy(username,argv[1]);

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

//	SETTING SERVER ATTRIBUTES
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) return 0;

	printf("CONNECTED TO THE SERVER\n");

//	INTRODUCE MYSELF WITH USERNAME
	send(clientSocket,username,1024,0);

	pthread_t thread;
	pthread_create(&thread, NULL, reception, (void *) &clientSocket );

//	CATCHES THE SIGINT SIGNAL
	signal(SIGINT, catch);

//  TAKES USER INPUT AND SEND IT ACCORDINGLY TO SERVER TO PARSE
	while(true){

		char input[1024];
		scanf("%s",input);

		if(strcmp(input,"ONEONE") == 0){

			send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			send(clientSocket,input,1024,0);
			
			scanf("%[^\n]s",input);
			send(clientSocket,input,1024,0);

		}
		if(strcmp(input,"BROADCAST") == 0){

			send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			send(clientSocket,input,1024,0);

			scanf("%[^\n]s",input);
			send(clientSocket,input,1024,0);

		}
		if(strcmp(input,"GROUPUS") == 0){

			send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			send(clientSocket,input,1024,0);

			int p=atoi(input);
			for( int i=0;i<p;i++)
			{
			scanf("%s",input);
			send(clientSocket,input,1024,0);
			}
		}
	}
}
