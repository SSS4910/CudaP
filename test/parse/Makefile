CC=gcc
WARN=-Wall

all: parse

parse: parse.o
	@echo "Linking object files..."
	@ $(CC) $(WARN) parse.o -o parse
	@echo "Done!"

parse.o:
	@echo "Creating parse.c object file..."
	@ $(CC) $(WARN) -c parse.c

objectclean:
	@echo "Removing object files..."
	@rm -f *.o

clean: objectclean
	@echo "Removing executables..."
	@rm -f ./parse
	@echo "Done!"
