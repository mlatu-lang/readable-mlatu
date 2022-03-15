CC=gcc

repl: mlatu.c repl.c
	$(CC) -O3 mlatu.c repl.c -o repl
