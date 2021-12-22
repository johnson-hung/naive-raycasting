all: raycasting

clean:
	rm -f *.o

PHONY: run

run:
	./raycasting

#================

raycasting: raycasting.o utils.o
	g++ -o raycasting raycasting.o utils.o

raycasting.o: raycasting.cpp utils.h stb_image.h
	g++ -c raycasting.cpp

utils.o: utils.cpp utils.h
	g++ -c utils.cpp