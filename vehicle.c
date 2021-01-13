#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"  

// GPS Data for this client as well as the connected tower ID


short  x;
short  y;
short  direction;
char  connectionID;
char  connectedTowerID;
char carIndex;




int main(int argc, char * argv[]) {
  
  // Set up the random seed
  srand(time(NULL));

  // Get the starting coordinate and direction from the command line arguments
  x = atoi(argv[1]);
  y = atoi(argv[2]);
  direction = atoi(argv[3]);

  // To start, this vehicle is not connected to any cell towers
  connectionID = -1;
  connectedTowerID = -1;

  // Go into an infinite loop to keep sending updates to cell towers
  while(1) {
    
    if((x<=0)||(x>=800)||(y<=0)||(y>=600)){
      exit(-1);
    }

    
    
    while(((x>0)&&(x<800)&&(y>0)&&(y<600))||(connectionID!=-1)){
      int ran = rand()%3+0;
      if(ran == 0){
        direction =direction + VEHICLE_TURN_ANGLE;
      }else if(ran == 1){
        direction =direction - VEHICLE_TURN_ANGLE;
      }

      usleep(100000);  // A delay to slow things down a little
      
       x += VEHICLE_SPEED*cos(VEHICLE_SPEED*cos((double)direction/180*M_PI));
       y += VEHICLE_SPEED*cos(VEHICLE_SPEED*cos((double)direction/180*M_PI));





      if(connectionID == -1){
        for (int i=0; i<7; i++){
          int                 clientSocket;  // client socket id
          struct sockaddr_in  clientAddress; // client address
          int                 status, bytesRcv;
          char                buffer[100];
          char                *buf;
          buf = buffer;
        
          int PORT = SERVER_PORT+i;
          clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
          if (clientSocket < 0) {
            printf("*** CLIENT ERROR: Could open socket.\n");
            exit(-1);
          }
          // Setup address
          memset(&clientAddress, 0, sizeof(clientAddress));
          clientAddress.sin_family = AF_INET;
          clientAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
          clientAddress.sin_port = htons((unsigned short) PORT);


          // Connect to server
          status = connect(clientSocket, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
          if (status < 0) {
            //printf("*** CLIENT ERROR: Could not connect.\n");
            exit(-1);
          }
          

          
          //buffer


          buffer[0] = CONNECT;
          buffer[1] = (x/256)+1;
          buffer[2] = (x%256)+1;
          buffer[3] = (y/256)+1;
          buffer[4] = (y%256)+1;
         

          //printf("CLIENT: Sending \"%s\" to server.\n", buffer);
          send(clientSocket, buffer, strlen(buffer), 0);
          char newbuffer[100];
          // Get response from server, should be "OK"
          bytesRcv = recv(clientSocket, newbuffer, sizeof(buffer), 0);
         
          if(newbuffer[0]==YES){
            //printf("CLIENT get back YES.\n");
            connectionID = 0;
            connectedTowerID = 256*((unsigned char) newbuffer[1]-1)+((unsigned char)newbuffer[2]-1);
            carIndex =  256*((unsigned char)newbuffer[3]-1)+((unsigned char)newbuffer[4]-1);
            close(clientSocket);  // Don't forget to close the socket !
            //printf("CLIENT: Shutting down.\n");
            break;
          }else{

            if(!((x>0)&&(x<800)&&(y>0)&&(y<600))){
              close(clientSocket);
              exit(-1);

            }
            connectionID = -1;
            close(clientSocket);  // close the socket !
            //printf("CLIENT: Shutting down.\n");
            continue;

          }
          
          

          
          
        }//end of for loop


      }

      

      if(connectionID == 0){
          int                 clientSocket;  // client socket id
          struct sockaddr_in  clientAddress; // client address
          int                 status, bytesRcv;
          char                buffer[100];
          char                *buf;
          buf = buffer;
          // Contact all the cell towers and ask them to shut down
          // ...
        
          int PORT = SERVER_PORT+connectedTowerID;
          clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
          if (clientSocket < 0) {
            //printf("*** 2CLIENT ERROR: Could open socket.\n");
            exit(-1);
          }
          // Setup address
          memset(&clientAddress, 0, sizeof(clientAddress));
          clientAddress.sin_family = AF_INET;
          clientAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
          clientAddress.sin_port = htons((unsigned short) PORT);


          // Connect to server
          status = connect(clientSocket, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
          if (status < 0) {
            //printf("*** 2CLIENT ERROR: Could not connect.\n");
            exit(-1);
          }
          

          
          
          buffer[0] = UPDATE;
          buffer[1] = (x/256)+1;
          buffer[2] = (x%256)+1;
          buffer[3] = (y/256)+1;
          buffer[4] = (y%256)+1;
          buffer[5] = (carIndex/256)+1;
          buffer[6] = (carIndex%256)+1;
          short z = x/256;

          //printf("CLIENT2: Sending \"%s\" to server.\n", buffer);
          send(clientSocket, buffer, strlen(buffer), 0);

          bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);

          if(buffer[0]==NO){
            connectionID = -1;
            connectedTowerID = -1;
            close(clientSocket);  // close the socket !
          }else{
            close(clientSocket);  // close the socket !
          }

         }
      

        
      
      
    }// loop
    

    
    
  }
}
