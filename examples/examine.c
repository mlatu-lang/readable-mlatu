/* examine.c - examine reductions of a mlatu-6 program
   usage: ./examine <# of rewrites> <terms>
   example usage: ./examine 30 '(A)(~+,~+<)+<' */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mlatu.h"
#include "../mlatuHelpers.c"
I parses(S s,T*ast) { E e=parseTerms(s,ast); FR(e.f); R !e.e; }
I pr(T ast) { S s=prettyTerms(ast); puts(s); FR(s); }
I main(I argc,S argv[]) { P(argc!=3,puts("usage: ./examine <# of rewrites> <terms>\nexample: ./examine 10 '(+<)+<'"));
	D r=newRoot(); T ast; parses(argv[2],&ast); DO(atoi(argv[1]),pr(ast);stepRewrite(r,&ast)); freeTerms(ast); freeRules(r); }
