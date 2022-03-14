CC=gcc

repl: mlatu.c repl.c
	$(CC) mlatu.c repl.c -o repl
