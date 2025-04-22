CC=gcc
optimization=-O2

repl: lib
	$(CC) $(optimization) mlatu.o repl-src/repl.c -o repl

lib:
	$(CC) $(optimization) -c mlatu.c -o mlatu.o

halt: lib
	$(CC) $(optimization) mlatu.o examples/halt.c -o halt

clean:
	rm -f mlatu.o repl halt repl.exe halt.exe
