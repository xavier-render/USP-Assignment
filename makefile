CC = gcc
OBJ = linkedList.o pipe.o
EXEC = pipesim
CFLAGS = -Wall

#--------PROGRAM-EXECUTEABLES-----------------------------------------
calculator: $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

#--------OBJECTS------------------------------------------------------
main.o: main.c linkedList.h
	$(CC) $(CFLAGS) main.c -c

linkedList.o: linkedList.c linkedList.h
	$(CC) $(CFLAGS) linkedList.c -c

#--------UTILS--------------------------------------------------------
clean:
	rm -f $(OBJ) $(EXEC)
