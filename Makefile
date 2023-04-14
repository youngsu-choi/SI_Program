OBJS=main.o
TARGET=app.out

main: main.o 
	gcc -o app.out main.c
main.o: main.c student.h average.h
	gcc -c main.c

clean:
	rm -f *.o
	rm app.out
