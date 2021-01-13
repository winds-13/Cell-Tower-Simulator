#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Handle client requests coming in through the server socket.  This code should run
// indefinitiely.  It should wait for a client to send a request, process it, and then
// close the client connection and wait for another client.  The requests that may be
// handles are SHUTDOWN, CONNECT and UPDATE.  A SHUTDOWN request causes the tower to
// go offline.   A CONNECT request contains 4 additional bytes which are the high and
// low bytes of the vehicle's X coordinate, followed by the high and low bytes of its
// Y coordinate.  If within range of this tower, the connection is accepted and a YES
// is returned, along with a char id for the vehicle and the tower id.   If UPDATE is
// received, the additional 4 byes for the (X,Y) coordinate are also received as well
// as the id of the vehicle.   Then YES is returned if the vehicle is still within
// the tower range, otherwise NO is returned.
void *handleIncomingRequests(void *ct) {
  CellTower *tower = ct;
  tower->online = '1';
  int PORT = SERVER_PORT+(tower->id);

  int                 serverSocket, clientSocket;
  struct sockaddr_in  serverAddress, clientAddr;
  int                 status, addrSize, bytesRcv;
  char                buffer[100];
  char*               buff;
  
  int                 tempCarID;
  
  
  // Create the server socket
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0) {
    //printf("*** SERVER ERROR: Could not open socket.\n");
    exit(-1);
  }

  // Setup the server address
  memset(&serverAddress, 0, sizeof(serverAddress)); // zeros the struct
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons((unsigned short) PORT);

  // Bind the server socket
  status = bind(serverSocket,  (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if (status < 0) {
    //printf("*** SERVER ERROR: Could not bind socket.\n");
    exit(-1);
  }

  // Set up the line-up to handle up to 5 clients in line
  status = listen(serverSocket, 5);
  if (status < 0) {
    //printf("*** SERVER ERROR: Could not listen on socket.\n");
    exit(-1);
  }

  // Wait for clients now
  while (1) {
    //usleep(100000);
    addrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize);
    if (clientSocket < 0) {
      //printf("*** SERVER ERROR: Could not accept incoming client connection.\n");
      exit(-1);
    }
    //printf("SERVER: Received client connection.\n");
    

    // Go into infinite loop to talk to client
    while (1) {
      // Get the message from the client
      bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);

      if (buffer[0] == SHUTDOWN){
        close(clientSocket);
        close(serverSocket);
      }

      if(buffer[0] == CONNECT){

        int length, height, tower_x, tower_y,tower_radius, dx, dy;
        float distance;
        length = 256*((unsigned char) buffer[1]-1)+((unsigned char)buffer[2]-1);
        height = 256*((unsigned char) buffer[3]-1)+((unsigned char)buffer[4]-1);
        tower_x = tower->x;
        tower_y = tower->y;
        tower_radius = tower->radius;
        dx = length - tower_x;
        dy = height - tower_y;
        distance = sqrt(dx*dx + dy*dy);

        if(tower->numConnectedVehicles == MAX_CONNECTIONS || distance > tower_radius){
          buffer[0] = NO;
          char *buf = buffer;
          send(clientSocket, buf, strlen(buf), 0);
          break;
        }

        ConnectedVehicle newCar;
        newCar.x = length;
        newCar.y = height;
        newCar.connected = 1;
        tower->numConnectedVehicles++;
        for (int i=0; i<MAX_CONNECTIONS; i++){
          
          if(tower->connectedVehicles[i].connected == 0){
            tower->connectedVehicles[i] = newCar;
            tower->connectedVehicles[i].connected = 1;
            tempCarID = i;
            
            break;
          }
           
        }
        char newbuffer[100];
        char *buf = newbuffer;
        //send back
        newbuffer[0] = YES;
        newbuffer[1] = ((tower->id)/256)+1;
        newbuffer[2] = ((tower->id)%256)+1;
        newbuffer[3] = ((tempCarID)/256)+1;
        newbuffer[4] = ((tempCarID)%256)+1;
        
        send(clientSocket, buf, strlen(buf), 0);
      }

      if(buffer[0] == UPDATE){
        
        int length, height, tower_x, tower_y,tower_radius, dx, dy;
        float distance;
        length = 256*((unsigned char) buffer[1]-1)+((unsigned char)buffer[2]-1);
        height =  256*((unsigned char)buffer[3]-1)+((unsigned char)buffer[4]-1);
        tempCarID = 256*((unsigned char) buffer[5]-1)+((unsigned char)buffer[6]-1);
        tower_x = tower->x;
        tower_y = tower->y;
        tower_radius = tower->radius;
        dx = length - tower_x;
        dy = height - tower_y;
        distance = sqrt(dx*dx + dy*dy);
        
        if(distance > tower_radius){  //no longer in range
         
          tower->numConnectedVehicles--;
          //printf("CLIENT: update: connection num = %d\n", tower->numConnectedVehicles);
          tower->connectedVehicles[tempCarID].connected = 0;
          buffer[0] = NO;
          char *buf = buffer;
          send(clientSocket, buf, strlen(buf), 0);

        }else{                       //still in range
          tower->connectedVehicles[tempCarID].x = length;
          tower->connectedVehicles[tempCarID].y = height;
          buffer[0] = YES;
          char *buf = buffer;
          send(clientSocket, buf, strlen(buf), 0);
          break;

        }

      }
      break;
    } 
    //printf("SERVER: Closing client connection.\n");
    close(clientSocket); // Close this client's socket
    
    
  }

  // close the sockets!
  close(serverSocket);
}