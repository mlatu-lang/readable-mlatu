// halt.c - find all infinitely looping programs of a certain length
// usage: ./halt <length>
// since the halting problem is... the halting problem, this uses the heuristic that if a program can still be rewritten after 1000 rewrites, or if it grows to over 100 terms [see the memory muncher (+,+<)+<], it loops forever.
// on 8 and above, will eventually be killed because of memory leaks... i should really fix those

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mlatu.h"
#include "../mlatuHelpers.c"
I parses(S s,T *ast) { E e=parseTerms(s,ast); FR(e.f); R !e.e; }
I big(T ast) { I i=0; MAP(ast,i++); R i>100; }
I small(T*ast,D r) { DO(1000,{P(stepRewrite(r,ast),1);P(big(*ast),0);}); R 0; }
I ipow(I b,I p) { I r=1; DO(p,r*=b); R r; }
S cs="()+-<>~,";
I main(I argc,S argv[]){ I l=atoi(argv[1]); D r=newRoot(); char s[l+1]; s[l]=0; T ast;
	DO(ipow(8,l), I j=i; DO(l,s[i]=cs[j%8];j/=8); parses(s,&ast)&&!small(&ast,r)&&puts(s); freeTerms(ast)); freeRules(r); }
