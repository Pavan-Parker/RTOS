#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h> 
#include<signal.h>

bool lastTime = false;
void catch()
{
	lastTime=true;
}
int main(int argc, char *argv[])
{
	int fd = 0;
	char buff[1024];
	
	if(argc<3)
	{
		printf("Less no of arguments !!");
		return 0;
	}

	//Setup Buffer Array
	memset(buff, '0',sizeof(buff));	

	//Create Socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
    	if(fd<0)
	{
		perror("Client Error: Socket not created succesfully");
		return 0;
	}
	
	//Structure to store details
	struct sockaddr_in server; 
	memset(&server, '0', sizeof(server)); 

	//Initialize	
	server.sin_family = AF_INET;
	server.sin_port =  htons(atoi(argv[2]));

	int in = inet_pton(AF_INET, argv[1], &server.sin_addr);
	if(in<0)
	{
		perror("Client Error: IP not initialized succesfully");
		return 0;
	}

	//Connect to given server	
	in = connect(fd, (struct sockaddr *)&server, sizeof(server));
	if(in<0)
	{
		perror("Client Error: Connection Failed.");
		return 0;
	}    
	
	bool firstTime= true ;	
	while(1)
	{
		signal(SIGINT, catch);
		if(firstTime)
		{
			bzero(buff,256);
			strcpy(buff,"sup?");
			in = send(fd,buff,strlen(buff),0);
		    if (in < 0) 
		    {
			 perror("\nClient Error: Writing to Server");
		    	return 0;
			}
			printf("connected to server\n"); 
			firstTime=false;	
		}
		else if(lastTime)
		{
			bzero(buff,256);
			strcpy(buff,"see ya!");
			in = send(fd,buff,strlen(buff),0);
		    if (in < 0) 
		    {
			 perror("\nClient Error: Writing to Server");
		    	return 0;
			}
			printf("connected to server\n"); 
		break;
		}
		else{
		printf("Please enter the message: ");
    		bzero(buff,256);
    		fgets(buff,255,stdin);
    		
		    printf("\nSending to SERVER: %s ",buff);
	
		/* Send message to the server */
    		in = send(fd,buff,strlen(buff),0);
		    if (in < 0) 
		    {
			 perror("\nClient Error: Writing to Server");
		    	return 0;
		    }
		    
		    bzero(buff,256);
		
		}
	}
	close(fd);
	return 0;
}
