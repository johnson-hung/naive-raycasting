all: raycasting

clean:
	rm -f *.o

PHONY: run

run:
	./raycasting

#================

raycasting: raycasting.o utils.o canvas.o map.o player.o
	g++ -o raycasting raycasting.o utils.o canvas.o map.o player.o

raycasting.o: raycasting.cpp stb_image.h utils.h canvas.h map.h player.h
	g++ -c raycasting.cpp

utils.o: utils.cpp utils.h
	g++ -c utils.cpp

canvas.o: canvas.cpp canvas.h
	g++ -c canvas.cpp

map.o: map.cpp map.h
	g++ -c map.cpp

player.o: player.cpp player.h
	g++ -c player.cpp