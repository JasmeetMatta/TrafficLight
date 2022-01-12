/* C/C++ Code by Jasmeet Singh Matta for network realisation and working of traffic Led light concept in parallel 
As the requirement is specified this code can be modiefied to fit that requirement 
As no physical device is specified this code is fully logical digitally 
For example we dont have physical button therefore using keybord interupt as button*/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<pthread.h>
#include <conio.h>


#define ANSI_RED_COLOR "\x1b[31m"
#define ANSI_YELLOW_COLOR "\x1b[33m"
#define ANSI_GREEN_COLOR "\x1b[32m"
#define ANSI_WHITE_COLOR "\x1b[37m"
#define ANSI_RESET_COLOR "\x1b[0m"


#define mainRed 100
#define mainGreen 200
#define mainYellow 300
#define pedRed 110
#define pedGreen 210
#define resetTimer 120

int state = mainRed;
int pedState = pedGreen;
int stateFlow = 0;
int t20 = 20000000;
int t10 = 10000000;

#define PORT 8080

int timeReset();

int startWinsock (void)
{
    WSADATA wsa;
    return (WSAStartup (MAKEWORD (2,2), & wsa));
}
void* changeStateMainLight(void *arg){
	switch (state){
		case mainRed:
			timeReset();
			printf("\033[31m" "RED MAIN LED ON" "\033[0m" "\n");
			printf(ANSI_WHITE_COLOR "YELLOW MAIN LED OFF" ANSI_RESET_COLOR "\n");
			printf(ANSI_WHITE_COLOR "GREEN MAIN LED OFF" ANSI_RESET_COLOR "\n");
			usleep(t20);
			pedState = pedRed;
			state = mainYellow;
			break;
		case mainYellow:
			if (stateFlow == 0){
				printf(ANSI_WHITE_COLOR "RED MAIN LED OFF " ANSI_RESET_COLOR "\n");
				printf(ANSI_YELLOW_COLOR "YELLOW MAIN LED ON" ANSI_RESET_COLOR "\n");
				printf(ANSI_WHITE_COLOR "GREEN MAIN LED OFF" ANSI_RESET_COLOR "\n");
				usleep(t10);
				state = mainGreen;
				stateFlow = 1;				
			}
			if (stateFlow ==1 ){
				printf(ANSI_WHITE_COLOR "RED MAIN LED OFF " ANSI_RESET_COLOR "\n");
				printf(ANSI_YELLOW_COLOR "YELLOW MAIN LED ON" ANSI_RESET_COLOR "\n");
				printf(ANSI_WHITE_COLOR "GREEN MAIN LED OFF" ANSI_RESET_COLOR "\n");
				usleep(t10);
				state = mainRed;
				pedState = pedGreen;
				stateFlow = 0;				
			}
			break;
		case mainGreen:
			printf(ANSI_WHITE_COLOR "RED MAIN LED OFF " ANSI_RESET_COLOR "\n");
			printf(ANSI_WHITE_COLOR "YELLOW MAIN LED OFF" ANSI_RESET_COLOR "\n");
			printf(ANSI_GREEN_COLOR "GREEN MAIN LED ON" ANSI_RESET_COLOR "\n");
			usleep(t20);
			break;
	}
}

void* changeStatePedLight(void *arg ){
	switch (pedState)
	{
	case pedGreen:
		printf(ANSI_WHITE_COLOR "RED PED LED OFF " ANSI_RESET_COLOR "\n");
		printf(ANSI_GREEN_COLOR "GREEN PED LED ON" ANSI_RESET_COLOR "\n");
		break;
	case pedRed:
		printf(ANSI_RED_COLOR "RED PED LED ON " ANSI_RESET_COLOR "\n");
		printf(ANSI_WHITE_COLOR "GREEN PED LED OFF" ANSI_RESET_COLOR "\n");
		break;
	}
}

int timeChange(){
	t20 = 10000000;
	t10 = 5000000;
}

int timeReset(){
	t20 = 20000000;
	t10 = 10000000;	
}

void interuptHandler(){
	pthread_t trafficLight1 , pedLight1;
	if (state == mainRed){
		pthread_create(&trafficLight1, NULL , changeStateMainLight, (void*)NULL);
		pthread_create(&pedLight1, NULL , changeStatePedLight, (void*)NULL);
		pthread_join(trafficLight1,NULL);
		pthread_join(pedLight1,NULL);	
	}
	else if (state != mainRed){
		timeChange();
	}
}

void* clinetServerCreation()
{
	long rc;
    SOCKET s , new_socket;
    rc = startWinsock ();
    struct sockaddr_in server,client;
    int c , valread,addrlen ;
	int MAXRECV = 1024;
    char *buffer,*message;
	fd_set readfds;

    if (rc != 0){
    	printf ("Error: startWinsock, error code:% d \n", rc);
		exit(EXIT_FAILURE);
    }
    printf ("Winsock started! \n");
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);	
	}

    printf("Socket created.\n");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("connect error");
		exit(EXIT_FAILURE);
	}
	
	printf("Connected\n");
	message = state+"/r/n";
	//message = "GET / HTTP/1.1\r\n\r\n";
	if( send(s , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		exit(EXIT_FAILURE);
	}
	puts("Data Send\n");
	addrlen = sizeof(struct sockaddr_in);

	pthread_t trafficLight , pedLight,clientInit;
	//pthread_create(&clientInit,NULL, clinetServerCreation, (void*)NULL);
	while(TRUE){
		//clinetServerCreation();
		//while(!(_kbhit())){
		pthread_create(&trafficLight, NULL , changeStateMainLight, (void*)NULL);
		pthread_create(&pedLight, NULL , changeStatePedLight, (void*)NULL);
		//pthread_create(&clientInit,NULL, clinetServerCreation, (void*)NULL);
		pthread_join(trafficLight,NULL);
		pthread_join(pedLight,NULL);
		message = state+"/r/n";

		FD_ZERO(&readfds);
		FD_SET(s, &readfds);
		//if client presend in read sockets             
		if (FD_ISSET( s , &readfds)) 
		{
			//get details of the client
			getpeername(s , (struct sockaddr*)&server , (int*)&addrlen);

			//Check if it was for closing , and also read the incoming message
			//recv does not place a null terminator at the end of the string (whilst printf %s assumes there is one).
			valread = recv( s , buffer, MAXRECV, 0);
			
			if( valread == SOCKET_ERROR)
			{
				int error_code = WSAGetLastError();
				if(error_code == WSAECONNRESET)
				{
					//Somebody disconnected , get his details and print
					printf("Host disconnected unexpectedly , ip %s , port %d \n" , inet_ntoa(server.sin_addr) , ntohs(server.sin_port));
					
					//Close the socket and mark as 0 in list for reuse
					closesocket(s);
				}
				else
				{
					printf("recv failed with error code : %d" , error_code);
				}
			}
			if ( valread == 0)
			{
				//Somebody disconnected , get his details and print
				printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(server.sin_addr) , ntohs(server.sin_port));
					
				//Close the socket and mark as 0 in list for reuse
				closesocket(s);
			}
				
			//Echo back the message that came in
			else
			{
				//add null character, if you want to use with printf/puts or other string handling functions
				buffer[valread] = '\0';
				printf("%s:%d - %s \n" , inet_ntoa(server.sin_addr) , ntohs(server.sin_port), buffer);
				send( s , message , valread , 0 );
			}
		} 
	}
	return 0;
}







void main(){
	pthread_t trafficLight , pedLight,clientInit;
	pthread_create(&clientInit,NULL, clinetServerCreation, (void*)NULL);
	//clinetServerCreation();
	//while(TRUE){
		//while(!(_kbhit())){
	//		pthread_create(&trafficLight, NULL , changeStateMainLight, (void*)NULL);
	//		pthread_create(&pedLight, NULL , changeStatePedLight, (void*)NULL);
	//		//pthread_create(&clientInit,NULL, clinetServerCreation, (void*)NULL);
	//		pthread_join(trafficLight,NULL);
	//		pthread_join(pedLight,NULL);
	//		//pthread_join(clientInit,NULL);
	//		//changeStateMainLight();
	//	//}
	//	//interuptHandler();
	//}/
	pthread_exit(NULL);
}

