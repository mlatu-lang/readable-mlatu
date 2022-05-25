// timer.c - multiples 100 by 100 in mlatu and prints the result
// needs to be used with a command-line utility like `time`
// obselete with the introduction of timer mode to the repl, but serves as an
// example of how to use the library
#include "../mlatu.h"
#include "../mlatuMacros.h"
#include <stdio.h>
I main() {
  D r = newRoot();
  parseFile("examples/nats.mlt", r);
  I i;
  T ast = parseTerms("100 100 mul", &i);
  rewrite(r, ast);
  PF("%s\n", prettyTerms(ast));
}
