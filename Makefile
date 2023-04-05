CC=gcc
BIN=project.bin
OBJ=main.o student.o teacher.o headmaster.o tool.o project.o
FLAG=-Wall -Werror
STD=-std=gnu99
LINK=-ls

all:$(OBJ)
	$(CC) -o $(BIN) $(OBJ) 
	
project.o: project.c project.h student.h teacher.h headmaster.h tool.h
	$(CC) $(STD) $(FLAG) -c project.c
main.o:main.c  project.h 
	$(CC) $(STD) $(FLAG) -c main.c
student.o: student.c student.h project.h tool.h teacher.h tool.h
	$(CC) $(STD) $(FLAG) -c student.c
teacher.o: teacher.c teacher.h student.h project.h tool.h
	$(CC) $(STD) $(FLAG) -c teacher.c
headmaster.o: headmaster.c headmaster.h teacher.h project.h tool.h
	$(CC) $(STD) $(FLAG) -c headmaster.c 
tool.o: tool.c tool.h
	$(CC) $(STD) $(FLAG) -c tool.c

clean:
	rm $(BIN) $(OBJ)



