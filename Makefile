CC=gcc
optimization=-O2

repl: lib
	$(CC) $(optimization) mlatu.o repl-src/repl.c -o repl

lib:
	$(CC) $(optimization) -c mlatu.c -o mlatu.o

bible: lib
	$(CC) $(optimization) mlatu.o examples/bible.c -o bible

examine: lib
	$(CC) $(optimization) mlatu.o examples/examine.c -o examine

clean:
	rm -f mlatu.o repl halt examine repl.exe halt.exe examine.exe
