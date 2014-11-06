CC= nvcc
ARCH= -arch=sm_20

all: sss4910

sss4910: sss4910.o parser.o debug.o cuda.o
	@echo Linking object files...
	@ $(CC) $(ARCH) sss4910.o parser.o debug.o cuda.o -o sss4910
	@echo DONE!

sss4910.o:
	@echo Generating sss4910.c object file...
	@ $(CC) $(ARCH) sss4910.cu -c 

parser.o:
	@echo Generating parser.c object file...
	@ $(CC) -c parser.c

debug.o:
	@echo Generating debug.c object file...
	@ $(CC) -c debug.c

cuda.o:
	@echo Generating cuda.cu objectfile...
	@ $(CC) $(ARCH) cuda.cu -c 

errorclean:
	@echo Removin errorlog.txt...
	@rm -f errorlog.txt

outputclean:
	@echo Removing output.txt file...
	@rm -f output.txt

objectclean:
	@echo Removing object files...
	@rm -f *.o

clean: objectclean outputclean errorclean
	@echo Removing executeable...
	@rm -f ./sss4910
	@echo DONE!
