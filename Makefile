CC=clang
optimization=-O2

repl: lib
	$(CC) $(optimization) mlatu.o repl-src/repl.c -o repl

lib:
	$(CC) $(optimization) -c mlatu.c -o mlatu.o

timer: lib
	$(CC) $(optimization) mlatu.o examples/timer.c -o timer

clean:
	rm -f mlatu.o repl timer repl.exe timer.exe
