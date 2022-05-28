CFLAGS= -Wextra -g -std=c99

build: main.o tree.o linkedList.o
	gcc main.o tree.o linkedList.o -o sd_fs

main.o: main.c
	gcc -c main.c $(CFLAGS)

tree.o: tree.c
	gcc -c tree.c $(CFLAGS)

linkedList.o: linkedList.c 
	gcc -c linkedList.c $(CFLAGS)

clean:
	rm -rf *.o sd_fs Mazilu_Tarsoaga_Tema3_SD.zip

run:
	./sd_fs < commands.in

valgrind:
	valgrind --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --verbose \
		   ./sd_fs < commands.in

pack:
	zip Mazilu_Tarsoaga_Tema3_SD.zip *.c *.h README.md Makefile