CC=gcc
WARN= -Wall

all: sss4910

sss4910: sss4910.o parser.o debug.o ppfile.o analysis.o
	@echo Linking object files...
	@ $(CC) $(WARN) sss4910.o parser.o debug.o ppfile.o analysis.o -o sss4910
	@echo DONE!

sss4910.o:
	@echo Generating sss4910.c object file...
	@ $(CC) $(WARN) -c sss4910.c

parser.o:
	@echo Generating parser.c object file...
	@ $(CC) $(WARN) -c parser.c

debug.o:
	@echo Generating debug.c object file...
	@ $(CC) $(WARN) -c debug.c

analysis.o:
	@echo Generating analysis.c object file...
	@ $(CC) $(WARN) -c analysis.c

ppfile.o:
	@echo Generating ppfile.c object file...
	@ $(CC) $(WARN) -c ppfile.c

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
