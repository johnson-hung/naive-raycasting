all: raycasting

clean:
	rm -f *.o

PHONY: run

run:
	./raycasting

#================

raycasting: raycasting.o
	g++ -o raycasting raycasting.o

raycasting.o: raycasting.cpp
	g++ -c raycasting.cpp
