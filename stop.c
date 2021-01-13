#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "simulator.h"


void main() {
  for (int i=0; i<7; i++){
    int                 clientSocket;  // client socket id
    struct sockaddr_in  clientAddress; // client address
    int                 status;
    unsigned char       command = SHUTDOWN; 
    char                buffer[10];
    // Contact all the cell towers and ask them to shut down
    // ...
  
    int PORT = 6000+i;
    printf("trying to send port %d\n", PORT);
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
      printf("*** CLIENT ERROR: Could not connect.\n");
      exit(-1);
    }
    
    //strcpy(buffer, '1');
    buffer[0] = command;
    
    printf("CLIENT: Sending \"%s\" to server.\n", buffer);
    send(clientSocket, buffer, strlen(buffer), 0);

    close(clientSocket);  // Don't forget to close the socket !
    printf("CLIENT: Shutting down.\n");

  }
}
