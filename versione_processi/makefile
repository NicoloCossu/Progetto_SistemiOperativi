game: game.o main.o pipes.o grafica.o
	gcc -o game game.o main.o pipes.o grafica.o -lncurses
grafica.o: grafica.c grafica.h game.h
	gcc -c -o grafica.o grafica.c -lncurses
pipes.o: pipes.c pipes.h game.h
	gcc -c -o pipes.o pipes.c -lncurses
game.o: game.c game.h pipes.h
	gcc -c -o game.o game.c -lncurses
main.o: main.c game.h pipes.h
	gcc -c -o main.o main.c -lncurses
clean:
	rm *.o
	rm game


