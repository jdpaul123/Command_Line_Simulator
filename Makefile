CFLAGS = -Wall -g
OBJECTS = main.o string_parser.o command.o

lab1: $(OBJECTS)
	gcc -o pseudo-shell $^

main.o: main.c
string_parser.o: string_parser.c
command.o: command.c

clean:
	rm -f $(OBJECTS) pseudo-shell
