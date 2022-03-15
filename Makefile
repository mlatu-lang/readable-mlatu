CC=gcc
optimization=-Ofast

repl: lib
	$(CC) $(optimization) mlatu.o repl.c -o repl

lib:
	$(CC) $(optimization) -c mlatu.c -o mlatu.o
