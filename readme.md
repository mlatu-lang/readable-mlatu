# Readable Mlatu 
## An implementation of Mlatu written in C
Some Quiche Eaters have been complaining that code here is illegible, so I have named it "Readable Mlatu" to assert their fallaciousness.  

## Compiling and running
Running `make lib` will compile the `mlatu.o` file.  
Works with gcc/clang, hasn't been tested with any other compilers.  

## Repl
Run `make repl` to make the repl.  
Any line that rewrites to `bye` will exit the repl.  
There are some repl-specific system commands:
- `]d`:     toggles debug mode on or off
- `]rules`: prints tree of all defined rules  

I would have prefixed them with `)` instead of `]`, but it would make the parser super messy.

## Docs
Documentation can be found in the header file [mlatu.h](mlatu.h).  
You can also look at [repl.c](repl.c) for an example, and in the [examples](examples/) folder.  

## License
readable-mlatu, like all mlatu-lang projects, is licensed under https://mlatu-lang.github.io/license/
