#export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"

program: main.o game_instance.o reference.o tile.o unit.o team.o game_features.o basics.o
	g++ -o program main.o game_instance.o reference.o tile.o unit.o team.o game_features.o basics.o -lSDL2 -lSDL2_image -lSDL2_ttf -lm

main.o : main.cpp
	g++ -c main.cpp

game_instance.o: game_instance.cpp
	g++ -c game_instance.cpp

reference.o: reference.cpp
	g++ -c reference.cpp

tile.o: tile.cpp
	g++ -c tile.cpp

unit.o: unit.cpp
	g++ -c unit.cpp

team.o : team.cpp
	g++ -c team.cpp

game_features.o : game_features.cpp
	g++ -c game_features.cpp

basics.o : basics.cpp
	g++ -c basics.cpp

clean:
	rm -f *.o program
