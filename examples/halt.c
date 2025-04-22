// halt.c - find all infinitely looping programs of a certain length
// usage: ./halt <length>
// since the halting problem is... the halting problem, this uses the heuristic that if a program can still be rewritten after 1000 rewrites, it loops forever.
// on 8 and above, will eventually be killed because of memory leaks... i should really fix those

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mlatu.h"
#include "../mlatuHelpers.c"
D r; I loops(S s) { T ast; E e=parseTerms(s,&ast); P(e.e,0); DO(1000,P(stepRewrite(r,&ast),0)); R 1; }
I ipow(I b,I p) { I r=1; DO(p, r*=b); R r; }
S cs="()+-<>~,";
I main(I argc,S argv[]){ I l=atoi(argv[1]); r=newRoot(); char s[l+1]; s[l]=0;
	DO(ipow(8,l), I j=i; DO(l,s[i]=cs[j/ipow(8,i)%8];); loops(s)&&puts(s)); }
