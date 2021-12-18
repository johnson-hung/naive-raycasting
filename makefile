all: raycasting

clean:
	rm -f *.o

PHONY: run

run:
	./raycasting

#================

raycasting: raycasting.o
	g++ -o raycasting raycasting.o

raycasting.o: raycasting.cpp stb_image.h
	g++ -c raycasting.cpp
