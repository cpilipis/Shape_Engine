CC = gcc
CC_FLAGS = -Wall -I. 

shape_engine: bodies.o libbodies.o drawing.o main.o loading.o simplestuff.o
	$(CC) $(CC_FLAGS) -o shape_engine bodies.o libbodies.o drawing.o main.o loading.o simplestuff.o -lallegro -lallegro_primitives -lm

%.o: %.c %.h
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm *.o
