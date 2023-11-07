game: game.o main.o comunicazione.o grafica.o
	gcc -o game game.o main.o comunicazione.o grafica.o -lncurses -pthread
grafica.o: grafica.c grafica.h game.h
	gcc -c -o grafica.o grafica.c  -lncurses  -pthread
comunicazione.o: comunicazione.c comunicazione.h game.h
	gcc -c -o comunicazione.o comunicazione.c  -lncurses  -pthread
game.o: game.c game.h comunicazione.h
	gcc -c -o game.o game.c  -lncurses  -pthread
main.o: main.c game.h comunicazione.h
	gcc -c -o main.o main.c  -lncurses  -pthread
clean:
	rm *.o
	rm game

