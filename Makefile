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

find: lib
	$(CC) $(optimization) mlatu.o examples/find.c -o find

clean:
	rm -f mlatu.o repl halt examine find repl.exe halt.exe examine.exe find.exe
