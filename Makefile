all: shell.c test.c
	gcc -o ronakShell shell.c 
	gcc -o test test.c
clean:
	$(RM) ronakShell
	$(RM) test