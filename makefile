all: app run

app:
	gcc -o ASCII_Invaders main.c -lncurses
	clear

run:
	./ASCII_Invaders
	