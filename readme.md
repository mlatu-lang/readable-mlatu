# Readable Mlatu 
## An implementation of Mlatu written in C
Some Quiche Eaters have been complaining that code here is illegible, so I have named it "Readable Mlatu" to assert their fallaciousness.  

## Compiling and running
Running `make lib` will compile the `mlatu.o` file.  
Run `make repl` to make the repl.  
Works with gcc/clang, hasn't been tested with any other compilers.  
Any line that rewrites to `bye` will exit the repl.

## Flags
Run with `-d` to enable debug mode.
This means that each rewrite will be printed separately.  

## Docs
Documentation can be found in the header file [mlatu.h](mlatu.h).  
You can also look at [repl.c](repl.c) for an example, and in the [examples](examples/) folder.  

## License
readable-mlatu, like all mlatu-lang projects, is licensed under https://mlatu-lang.github.io/license/
