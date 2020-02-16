#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int userCount;
	int userPool[100];
	int fd = 0;
	char buff[1024];
	char message[1024];
	char *ip; 	
	//Setup Buffer Array
	memset(buff, '0',sizeof(buff));	
	memset(message, '0',sizeof(message));	
	//Create Socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
    	if(fd<0)
	{
		perror("Client Error: Socket not created succesfully");
		return 0;
	}

	//Structure to store details
	struct sockaddr_in server, client; 
	 socklen_t addr_size; 
	 addr_size = sizeof(struct sockaddr_in); 
    
	memset(&server, '0', sizeof(server)); 

	//Initialize	
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2])); 
	int in = inet_pton(AF_INET, argv[1], &server.sin_addr.s_addr);
        //server.sin_addr.s_addr = htonl(INADDR_ANY);
   

	bind(fd, (struct sockaddr*)&server, sizeof(server)); 

//	int in;
	
	listen(fd, 10); 
	char sup[]="sup?";
	char bye[]="see ya!";
	while(	in = accept(fd, (struct sockaddr*)&client, &addr_size))
	{		
		int childpid,n;
		if ( (childpid = fork ()) == 0 ) 
		{
		
			//printf ("\nOne Client Connected !! ");
		
			//close listening socket
		
			//Clear Zeroes
			bzero(buff,256);
			bzero(message,256);
			printf("Connection Established\n"); 
        	char ip[INET_ADDRSTRLEN]; 
       		inet_ntop(AF_INET, &(client.sin_addr), ip, INET_ADDRSTRLEN); 
      
       		printf("connection established with IP : %s and PORT : %d\n", ip, ntohs(client.sin_port)); 				
			while ( (n = recv(in, buff, 256,0)) >0)  
			{
				printf("Server Received: %s \n", buff);
				if(strcmp(buff,"sup?")){

					sprintf(message,"User %d joined",userCount);
					userCount++;
				}
				else if(strcmp(buff,bye)){
					sprintf(message,"User %d left",userCount);
					userCount--;
				}
				else
				{
					strcpy(message,buff);
				}
				printf("Server Received: %s", message);
				
	
				bzero(buff,256);
										
				bzero(message,256);
			}
			close(in);
			exit(0);
		}

	}
}
