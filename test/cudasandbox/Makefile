CC=gcc
WARN=-Wall

all: cudasandbox.o
	@echo Linking object files...
	@$(CC) $(WARN) cudasandbox.o -o cudasandbox
	@echo DONE!

cudasandbox.o:
	@echo Generating cudasandbox.c object file... 
	@$(CC) $(WARN) -c cudasandbox.c

objectclean:
	@echo Removing object files...
	@rm -f *.o

clean: objectclean
	@echo Removing executable file...
	@rm -f ./cudasandbox
	@echo DONE!
