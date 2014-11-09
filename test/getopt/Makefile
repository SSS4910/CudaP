all: gopt

gopt: gopt.o
	@echo Linking Object Files...
	@echo Creating executeable...
	@gcc gopt.o -o gopt
	@echo DONE!!!

gopt.o:
	@echo Creating gopt.c object file...
	@gcc -c gopt.c

clean:
	@echo Cleaning up
	@rm -f ./gopt
	@rm -f *.o
	@echo DONE!!!
