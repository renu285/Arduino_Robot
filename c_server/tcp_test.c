/*
    C socket server example
*/
 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

char cmd[32];
char client_message[2000];

int send_cmd(int client_sock,char *cmd_str,int arg) {
    
    int ret = 0;

    memset(cmd,'\0',sizeof(cmd));
    if(!arg) 
        sprintf(cmd,"%s",cmd_str);
    else
        sprintf(cmd,"%s %d",cmd_str,arg);
    ret = write(client_sock,cmd,strlen(cmd));

    return ret;
}

int rcv_client(int client_sock) {

    int ret = 0;
    ret = recv(client_sock , client_message , 2000 , 0);
    return atoi(client_message);

}



 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    int cmd_val= 0; 
    int val;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8089 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");


        //Listen
        listen(socket_desc , 3);

        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");
        
        printf("Waiting for client to be initialized \n");

        while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
        {
            printf("Client sent : %s\n",client_message);
            if(!strcmp(client_message,"INIT DONE")) 
                printf("Client initialization complete\n");
            break;
        }

        //Receive a message from client
        while(1) {     

         printf("Enter the command(1-13) to the bot : \n");
         printf(" 1. Move forward \n"); 
         printf(" 2. Move forward for time in ms \n"); 
         printf(" 3. Move forward for distance  \n"); 
         printf(" 4. Move reverse \n"); 
         printf(" 5. Move reverse for time in ms \n"); 
         printf(" 6. Move reverse for distance  \n"); 
         printf(" 7. Move left \n"); 
         printf(" 8. Move left Degrees\n"); 
         printf(" 9. Move right \n"); 
         printf(" 10. Move right Degrees\n"); 
         printf(" 11. Stop the bot\n"); 
         printf(" 12. Get obstacle distance front\n"); 
         printf(" 13. Get obstacle distance left\n"); 
         printf(" 14. Get obstacle distance right\n"); 
         printf(" 15. Get magnetometer data\n"); 
         printf(" 16. Get RSSI value\n\n");
         printf("  Waiting for user input : "); 

         scanf("%d",&cmd_val);        

         switch(cmd_val) {

             case 1:
                 send_cmd(client_sock,"forward",0);
                 //write(client_sock,"forward",strlen("forward"));
                 break;
             case 2:
                 printf("Enter the time in ms : \n");
                 scanf("%d",&val);
                 send_cmd(client_sock,"forward time",val);
                 //memset(cmd,'\0',sizeof(cmd));
                 //sprintf(cmd,"forward time %d",val);
                 //write(client_sock,cmd,strlen(cmd));
                 break;
             case 3:
                 printf("Enter the distance: \n");
                 scanf("%d",&val);
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"forward distance %d",val);
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 4:
                 write(client_sock,"reverse",strlen("reverse"));
                 break;
             case 5:
                 printf("Enter the time in ms : \n");
                 scanf("%d",&val);
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"reverse time %d",val);
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 6:
                 printf("Enter the distance: \n");
                 scanf("%d",&val);
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"reverse distance %d",val);
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 7:
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"left");
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 8:
                 printf("Enter Degrees: \n");
                 scanf("%d",&val);
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"left deg %d",val);
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 9:
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"right");
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 10:
                 printf("Enter Degrees: \n");
                 scanf("%d",&val);
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"right deg %d",val);
                 write(client_sock,cmd,strlen(cmd));
                 break;
             case 11:
                 memset(cmd,'\0',sizeof(cmd));
                 sprintf(cmd,"stop");
                 write(client_sock,cmd,strlen(cmd));
                
             case 12:
                 memset(cmd,'\0',sizeof(cmd));
                 memset(client_message,'\0',sizeof(client_message));
                 sprintf(cmd,"distance front");
                 write(client_sock,cmd,strlen(cmd));
                 printf("Waiting to retreive info from front obstacle sensor\n");
                 printf("Client sent : %d\n",rcv_client(client_sock));
                 break;
             case 13:
                 memset(cmd,'\0',sizeof(cmd));
                 memset(client_message,'\0',sizeof(client_message));
                 sprintf(cmd,"distance left");
                 write(client_sock,cmd,strlen(cmd));
                 printf("Waiting to retreive info from left obstacle sensor\n");
                 recv(client_sock , client_message , 2000 , 0);
                 printf("Client sent : %s\n",client_message);
                 break;
             case 14:
                 memset(cmd,'\0',sizeof(cmd));
                 memset(client_message,'\0',sizeof(client_message));
                 sprintf(cmd,"distance right");
                 write(client_sock,cmd,strlen(cmd));
                 printf("Waiting to retreive info from right obstacle sensor\n");
                 recv(client_sock , client_message , 2000 , 0);
                 printf("Client sent : %s\n",client_message);
                 break;
             case 15:
                 memset(cmd,'\0',sizeof(cmd));
                 memset(client_message,'\0',sizeof(client_message));
                 sprintf(cmd,"magval");
                 write(client_sock,cmd,strlen(cmd));
                 printf("Waiting to retreive info from magnetometer\n");
                 recv(client_sock , client_message , 2000 , 0);
                 printf("Client sent : %s\n",client_message);
                 break;
             case 16:
                 memset(cmd,'\0',sizeof(cmd));
                 memset(client_message,'\0',sizeof(client_message));
                 sprintf(cmd,"rssi");
                 write(client_sock,cmd,strlen(cmd));
                 printf("Waiting to retreive RSSI info \n");
                 recv(client_sock , client_message , 2000 , 0);
                 printf("Client sent : %s\n",client_message);
                 break;
             default:
                 printf("Unknown command received\n");
                 break;

         }
            system("clear");
        }

    return 0;
}
