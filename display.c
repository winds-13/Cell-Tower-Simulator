#include <unistd.h>
#include <X11/Xlib.h>

//Display-related variables
Display *display;
Window   win;
GC       gc;


// Initialize and open the simulator window with size CITY_WIDTH x CITY_HEIGHT.
void initializeWindow() {
  // Open connection to X server
  display = XOpenDisplay(NULL);

  // Create a simple window, set the title and get the graphics context then
  // make is visible and get ready to draw
  win = XCreateSimpleWindow(display,  RootWindow(display, 0), 0, 0,
			    CITY_WIDTH, CITY_HEIGHT, 0, 0x000000, 0xFFFFFF);
  XStoreName(display, win, "Vehicle Simulator");
  gc = XCreateGC(display, win, 0, NULL);
  XMapWindow(display, win);
  XFlush(display);
  usleep(20000);  // sleep for 20 milliseconds.
}

// Close the display window
void closeWindow() {
  XFreeGC(display, gc);
  XUnmapWindow(display, win);
  XDestroyWindow(display, win);
  XCloseDisplay(display);
}


// Redraw all the cell towers and all the vehicles that are connected to towers.
// This code should run in an infinite loop continuously drawing the city.
// Vehicles are drawn as circles with radius VEHICLE_RADIUS.
void *showSimulation(void *c) {
  City  *city = c;
  
  // Open the window
  initializeWindow();

  // Now keep redrawing until someone kills the thread
  while(1) {
    // Erase the background 
    XSetForeground(display, gc, 0xFFFFFF);
    XFillRectangle(display, win, gc, 0, 0, CITY_WIDTH, CITY_HEIGHT);

    // Draw all the cell towers
    for (int i=0; i<city->numTowers; i++) {
      short r = city->towers[i].radius;
      XSetForeground(display, gc, city->towers[i].color);
      for (int b=-3;b<=3; b++)
	XDrawArc(display, win, gc,
		 city->towers[i].x-r+b, city->towers[i].y-r+b,
		 2*(r-b), 2*(r-b), 0, 360*64);
    }
    
    // Draw all the vehicles within each cell tower's range 
    for (int t=0; t<city->numTowers; t++) {
      for (int i=0; i<MAX_CONNECTIONS; i++) {
        
	if (city->towers[t].connectedVehicles[i].connected == 1) {
   // printf("display!!!!!!!\n");
	  XSetForeground(display, gc, city->towers[t].color);
	  XFillArc(display, win, gc,
		   city->towers[t].connectedVehicles[i].x-VEHICLE_RADIUS,
		   city->towers[t].connectedVehicles[i].y-VEHICLE_RADIUS,
		   2*VEHICLE_RADIUS, 2*VEHICLE_RADIUS, 0, 360*64);
	  XSetForeground(display, gc, 0x000000); // draw a black border
	  XDrawArc(display, win, gc,
		   city->towers[t].connectedVehicles[i].x-VEHICLE_RADIUS,
		   city->towers[t].connectedVehicles[i].y-VEHICLE_RADIUS,
		   2*VEHICLE_RADIUS, 2*VEHICLE_RADIUS, 0, 360*64);
	}
      }
    }
    XFlush(display);
    usleep(2000);
  }
  
  closeWindow();
  pthread_exit(NULL);
}
