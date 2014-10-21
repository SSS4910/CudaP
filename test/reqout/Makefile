CC=gcc
WARN=-Wall

all: template

template: template.o
	@echo "Linking object files..."
	@ $(CC) $(WARN) template.o ./../../src/requesttofile.o ./../../src/debug.o -o template
	@echo "Done!"

template.o:
	@echo "Creating template.c object file..."
	@ $(CC) $(WARN) -c template.c

outputclean:
	@echo "Removing output.txt file..."
	@rm -f output.txt

objectclean:
	@echo "Removing object files..."
	@rm -f *.o

clean: objectclean outputclean
	@echo "Removing executables..."
	@rm -f ./template
	@echo "Done!"
