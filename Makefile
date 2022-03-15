CC=gcc

repl: mlatu.c repl.c
	$(CC) -Ofast mlatu.c repl.c -o repl
