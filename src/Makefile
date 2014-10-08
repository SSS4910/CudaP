CC=gcc
WARN= -Wall

all: sss4910

sss4910: sss4910.o accessreader.o cudaparser.o
	@echo Linking object files...
	@ $(CC) $(WARN) sss4910.o accessreader.o cudaparser.o -o sss4910
	@echo DONE!

sss4910.o:
	@echo Generating sss4910.c object file...
	@ $(CC) $(WARN) -c sss4910.c

accessreader.o:
	@echo Generating accessreader.c object file...
	@ $(CC) $(WARN) -c accessreader.c

cudaparser.o:
	@echo Generating cudaparser.c object file...
	@ $(CC) $(WARN) -c cudaparser.c

objectclean:
	@echo Removing object files...
	@rm -f *.o

clean: objectclean
	@echo Removing executeable...
	@rm -f ./sss4910
	@echo DONE!
