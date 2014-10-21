CC=gcc
WARN=-Wall

all: template

template: template.o
	@echo "Linking object files..."
	@ $(CC) $(WARN) template.o -o template
	@echo "Done!"

template.o:
	@echo "Creating template.c object file..."
	@ $(CC) $(WARN) -c template.c

objectclean:
	@echo "Removing object files..."
	@rm -f *.o

clean: objectclean
	@echo "Removing executables..."
	@rm -f ./template
	@echo "Done!"
