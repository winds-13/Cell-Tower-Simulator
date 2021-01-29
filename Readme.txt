a real-time cell tower simulator with a graphical interface modelling the movement of cars driving in a city and connecting to one of seven cell towers when within their radius using threads, signals and TCP sockets in C and Concurrent Computing concepts 

name:
	Henry Tu

files:
	cellTower.c
	display.c
	generator.c
	makefile
	Readme.txt
	simulator.c
	simulator.h
	vehicle.c
	stop.c


1. compile by "make"
2. "simulator &"
3. "generator &"
4. use "fg" bring generator to foreground and kill it
5. wait for all vehicles leave, then "./stop"clear./"
