CFLAGS=-ggdb -O0 -std=c99 -Wall -Wconversion -Wtype-limits -Werror -I src
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2

valgrind: main
	valgrind $(VFLAGS) ./main 2>&1

pruebas: main.c src/*.c src/*.h
	gcc $(CFLAGS) -o main main.c src/*.c 2>&1


valgrind-pruebas: pruebas
	valgrind $(VFLAGS) ./pruebas 2>&1

pruebas: pruebas.c src/*.c src/*.h
	gcc $(CFLAGS) -o pruebas pruebas.c src/*.c 2>&1

clean:
	rm pruebas 2>&1
