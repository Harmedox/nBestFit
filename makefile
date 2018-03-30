HEADERS = 

default: main

main: main.c
	gcc main.c -o main -lm -std=c99

clean:
	-rm -f main
