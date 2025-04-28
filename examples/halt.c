// halt.c - find all infinitely looping programs of a certain length
// usage: ./halt <length>
// since the halting problem is... the halting problem, this uses the heuristic that if a program can still be rewritten after 1000 rewrites, or if it grows to include over 1000 terms [see the memory munchers (+,+<)+< and (A)(~+,~+<)+<], it loops forever.
// on 8 and above, will eventually be killed because of memory leaks... i should really fix those

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mlatu.h"
#include "../mlatuHelpers.c"
I parses(S s,T *ast) { E e=parseTerms(s,ast); FR(e.f); R !e.e; }
I size(T ast) { I i=0; MAP(ast,i++;!c->w&&(i+=size(c->c))); R i; }
I small(T*ast,D r) { DO(1000,{P(stepRewrite(r,ast),1);P(size(*ast)>1000,0);}); R 0; }
I comb(T ast) { P(!ast,0); MAP(ast,P(c->w?!strchr("ABCDE",*c->w):!comb(c->c),0)); R 1; }
I ipow(I b,I p) { I r=1; DO(p,r*=b); R r; }
I pr(T ast) { S s=prettyTerms(ast); puts(s); FR(s); }
S cs="()+-<>~,";
I oflen(I l) { char s[l+16]; s[l+15]=0; strcpy(s,"(E)(D)(C)(B)(A)"); T ast; D r=newRoot();
	DO(ipow(8,l), I j=i; DO(l,s[i+15]=cs[j%8];j/=8); parses(s,&ast)&&small(&ast,r)&&comb(ast)&&PF("%s|",s)&&pr(ast); freeTerms(ast)); freeRules(r); }
I main(I argc,S argv[]){ DO(1+atoi(argv[1]),oflen(i)); }
