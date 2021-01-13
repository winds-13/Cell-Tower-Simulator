#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "simulator.h"
#include <sys/wait.h> /* for wait */
void main() {
  // Set up the random seed
  
  srand(time(NULL));

 while(1) {
    for (int i=0; i<5; i++) {
      // Start off with a random location and direction
      short x = (int)(rand()/(double)(RAND_MAX)*CITY_WIDTH);
      short y = (int)(rand()/(double)(RAND_MAX)*CITY_HEIGHT);
      short direction = (int)((rand()/(double)(RAND_MAX))*360 - 180);
      char buffer[100];
      int snp = snprintf(buffer, sizeof(buffer), "%s %d %d %d %s", "./vehicle", x, y, direction, "&");
      buffer[sizeof(buffer)] = '\0';
      int i = system(buffer);
     }
       sleep(1);
  }
}