all:         simulator vehicle generator stop
	     gcc -o simulator simulator.o  -lX11 -lpthread -lm
	     gcc -o vehicle vehicle.o -lm 
	     gcc -o generator generator.o 
	     gcc -o stop stop.o 

simulator:   simulator.c display.c cellTower.c simulator.h
	     gcc -c simulator.c

vehicle:     vehicle.c simulator.h
	     gcc -c vehicle.c

generator:   generator.c simulator.h
	     gcc -c generator.c

stop:        stop.c simulator.h
	     gcc -c stop.c

clean:
	     rm -f simulator.o vehicle.o generator.o stop.o simulator vehicle generator stop