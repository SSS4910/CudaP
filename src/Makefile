CC=gcc
WARN= -Wall

all: sss4910

sss4910: sss4910.o accessreader.o cudaparser.o debug.o
	@echo Linking object files...
	@ $(CC) $(WARN) sss4910.o accessreader.o cudaparser.o debug.o -o sss4910 -lpthread
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

debug.o:
	@echo Generating debug.c object file...
	@ $(CC) $(WARN) -c debug.c

errorclean:
	@echo Removin errorlog.txt...
	@rm -f errorlog.txt

objectclean:
	@echo Removing object files...
	@rm -f *.o

clean: objectclean errorclean
	@echo Removing executeable...
	@rm -f ./sss4910
	@echo DONE!
