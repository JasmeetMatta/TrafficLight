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

#pragma comment(lib, "ws2_32.lib")

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

int timePgReset();

int startWinsock (void)
{
    WSADATA wsa;
    return (WSAStartup (MAKEWORD (2,2), & wsa));
}    
void* changeStateMainLight(void *arg){
	switch (state){
		case mainRed:
			timePgReset									();
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

int timePgReset(){
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


void* serverCreation()
{
	WSADATA wsa;
	SOCKET master , new_socket , client_socket[3] , s;
	struct sockaddr_in server, address;
	int max_clients = 3 , activity, addrlen, i, valread;
	char *message = state+"/r/n";
	//size of our receive buffer, this is string length.
	int MAXRECV = 1024;
    fd_set readfds;
	char *buffer;
	buffer =  (char*) malloc((MAXRECV + 1) * sizeof(char));

	long rc;

	for(i = 0 ; i < 30;i++)
	{
		client_socket[i] = 0;
	}
	rc = startWinsock();
	printf("\nInitialising Winsock...");
	if (rc != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if((master = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf("Socket created.\n");
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	
	//Bind
	if( bind(master ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	puts("Bind done");

	//Listen to incoming connections
	listen(master , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	
	addrlen = sizeof(struct sockaddr_in);
    pthread_t trafficLight , pedLight, serverInit;
	while(TRUE)
    {
		pthread_create(&trafficLight, NULL , changeStateMainLight, (void*)NULL);
		pthread_create(&pedLight, NULL , changeStatePedLight, (void*)NULL);
		//pthread_create(&clientInit,NULL, clinetServerCreation, (void*)NULL);
		pthread_join(trafficLight,NULL);
		pthread_join(pedLight,NULL);		
		message = state+"/r/n";
        //clear the socket fd set
        FD_ZERO(&readfds);
 
        //add master socket to fd set
        FD_SET(master, &readfds);
        
        //add child sockets to fd set
        for (  i = 0 ; i < max_clients ; i++) 
        {
            s = client_socket[i];
            if(s > 0)
            {
                FD_SET( s , &readfds);
            }
        }
		
        activity = select( 0 , &readfds , NULL , NULL , NULL);
   
        if ( activity == SOCKET_ERROR ) 
        {
            printf("select call failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
        }
         
        if (FD_ISSET(master , &readfds)) 
        {
            if ((new_socket = accept(master , (struct sockaddr *)&address, (int *)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
         
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
       
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send failed");
            }
             
            puts("Welcome message sent successfully");
             
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets at index %d \n" , i);
                    break;
                }
            }
        }
         
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            s = client_socket[i];
			//if client presend in read sockets             
            if (FD_ISSET( s , &readfds)) 
            {
                //get details of the client
				getpeername(s , (struct sockaddr*)&address , (int*)&addrlen);

				//Check if it was for closing , and also read the incoming message
				//recv does not place a null terminator at the end of the string (whilst printf %s assumes there is one).
                valread = recv( s , buffer, MAXRECV, 0);
				
				if( valread == SOCKET_ERROR)
				{
					int error_code = WSAGetLastError();
					if(error_code == WSAECONNRESET)
					{
						//Somebody disconnected , get his details and print
						printf("Host disconnected unexpectedly , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                     
						//Close the socket and mark as 0 in list for reuse
						closesocket( s );
						client_socket[i] = 0;
					}
					else
					{
						printf("recv failed with error code : %d" , error_code);
					}
				}
				if ( valread == 0)
                {
                    //Somebody disconnected , get his details and print
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                     
                    //Close the socket and mark as 0 in list for reuse
                    closesocket( s );
                    client_socket[i] = 0;
                }
                 
                //Echo back the message that came in
                else
                {
					//add null character, if you want to use with printf/puts or other string handling functions
					buffer[valread] = '\0';
					printf("%s:%d - %s \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port), buffer);
					send( s , message , valread , 0 );
                }
            }
        }
    }


	/*
    long rc;
    SOCKET s , new_socket;
    rc = startWinsock ();
    struct sockaddr_in server,client;
    int c;
    char *message,conv;
    if (rc != 0){
    printf ("Error: startWinsock, error code:% d \n", rc);
    }
    printf ("Winsock started! \n");
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(PORT);

    if(bind(s,(struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
        printf("BINDING FAILED");
        return 1;
    }
    printf("BIND DONE");
    listen(s, 6);

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET){
        printf("ACCEPT FAILED WIT ERROR %d",WSAGetLastError());
        return 1;
    }
    printf("Connection accepted");
	while(1){
		message = "HEY HEY HEY ";
		send(new_socket, message, strlen(message),0);
		printf("Message Sent");
	}
	//conv = char(state+'0');
*/
}
    





void main(){
	pthread_t trafficLight , pedLight, serverInit;
	pthread_create(&serverInit, NULL, serverCreation, (void*)NULL);
	//serverCreation();
	//while(TRUE){
		//while(!(_kbhit())){
	//		pthread_create(&trafficLight, NULL , changeStateMainLight, (void*)NULL);
	//		pthread_create(&pedLight, NULL , changeStatePedLight, (void*)NULL);
      //      //pthread_create(&serverInit, NULL, serverCreation, (void*)NULL);
		//	pthread_join(trafficLight,NULL);
		//	pthread_join(pedLight,NULL);
            //pthread_join(serverInit,NULL);
			//changeStateMainLight();
		//}
		//interuptHandler();
	//}
	pthread_exit(NULL);
}
