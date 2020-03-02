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

int clientCount = 0;
int groupCount = 0;


static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct client{
	int index;
	char username[1024];
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};

struct group{

	int index;
	int no_people;
	char username_g[1024];
	int people[1024];

};


struct client Client[1024];
struct group Group[1024];

pthread_t thread[1024];

void * reception(void * attrClient){

//	SETTING CLIENT ATTRIBUTES
	struct client* attrClient = (struct client*) attrClient;
	int index = attrClient -> index;
	int clientSocket = attrClient -> sockID;
	char username[1024];
		
//	RECEIVE FIRST MESSAGE FROM CLIENT WHICH IS HIS USERNAME
	recv(clientSocket,username,1024,0);
	strcpy(Client[index].username,username);
	printf("USER %s JOINED AND ID : %d IS ALLOTED.\n",username,index + 1);
	
//	PARSING THE CLIENT'S REQUEST	
	while(1){
		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';

		char output[1024];

//		IF CLIENT WANTS TO SEND A ONE TO ONE MESSAGE

		if(strcmp(data,"ONEONE") == 0){
			read = recv(clientSocket,data,1024,0);
			int id=1025;
			for(int i = 0 ; i < clientCount; i ++){			
				if (strcmp(data, (Client[i].username)) ==0){id = i;}
			}
			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';

			if (id != 1025)
			{
				char tem[1024];
				strcpy(tem,username);
				strcat(tem,":");
				strcat(tem,data);				
				send(Client[id].sockID,tem,1024,0);			
			}
			else
			{	
				send(Client[id].sockID,"NOT VALID",1024,0);
			}


		}

//		IF CLIENT WANTS TO SEND A GROUP MESSAGE
		
		if(strcmp(data,"BROADCAST") == 0){			
			read = recv(clientSocket,data,1024,0);

			int id=1025;

			for(int i = 0 ; i < groupCount; i ++){			
				If ( strcmp(data, (Group[i].username_g) ) ==0) {id = i;}
			}
		
			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';

			if (id != 1025)
			{
				int k=0;			
				for(int i = 0 ; i <= Group[id].no_people; i ++){
					if (index == (Group[id].people)[i])	{k ++;}
				}
			
				if (k>=1)
				{
					char tem[100];
					strcpy(tem,username);
					strcat(tem,"(");
					strcat(tem,Group[id].username_g);
					strcat(tem,")");				
					strcat(tem,":");
					strcat(tem,data);
					for(int i = 0 ; i < Group[id].no_people; i ++){
						send(Client[(Group[id].people)[i]-1].sockID,tem,1024,0);
					}
				}
			}
			else{send(Client[index].sockID,"NOT VALID GROUP",1024,0);}

		}
//		IF CLIENT WANTS TO CREATE A GROUP

		if(strcmp(data,"GROUPUS") == 0){
			read = recv(clientSocket,data,1024,0);
			strcpy(Group[groupCount].username_g,data);
			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';


			int num_of_group = atoi(data);
			Group[groupCount].index=groupCount;
			Group[groupCount].no_people=num_of_group;


			for(int i = 0 ; i < num_of_group ; i ++){
				read = recv(clientSocket,data,1024,0);
				data[read] = '\0';
				(Group[groupCount].people)[i]=atoi(data);
			}						
		groupCount ++;
		
		}



	}

	return NULL;

}

int main(int argc,char *argv[]){

	int port;
	port = atoi(argv[1]);
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);

//	SERVER ATTRIBUTES
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
//	BIND TO PORT
	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) return 0;

//	LISTEN TO INCOMING MESSAGES
	if(listen(serverSocket,1024) == -1) return 0;

	printf("INITIALIZED AT PORT %d\n",port);

//	IF ANY CONNECTION COMES IT CREATES A NEW THREAD FOR IT WITH FUNCTION "reception" ASSIGNED

	while(1){
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;
		pthread_create(&thread[clientCount+groupCount], NULL, reception, (void *) &Client[clientCount]);
		clientCount ++;
	}

	for(int i = 0 ; i < clientCount ; i ++)
		pthread_join(thread[i],NULL);

}
