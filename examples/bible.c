/* bible.c
   print the mlatu bible - all combinators up to a certain length
   usage: ./bible <length>
   if saving a large number of combinators to a file, you may want to pretty-print without whitespace, to save memory.
   that can be done by modifying mlatu.c like this: V prTL(T t) { MAP(t,prT(c)); } */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mlatu.h"
#include "../mlatuHelpers.c"
I parses(S s,T*ast) { E e=parseTerms(s,ast); FR(e.f); R !e.e; }
I size(T ast) { I i=0; MAP(ast,i++;!c->w&&(i+=size(c->c))); R i; }
I small(T*ast,D r) { DO(1000,P(stepRewrite(r,ast),1);P(size(*ast)>1000,0)); R 0; }
I comb(T ast) { P(!ast,0); MAP(ast,P(c->w?!strchr("ABCDE",*c->w):!comb(c->c),0)); R 1; }
I pr(T ast) { S s=prettyTerms(ast); puts(s); FR(s); }
I ipow(I b,I p) { I r=1; DO(p,r*=b); R r; }
S cs="()+-<>~,", prelude="(E)(D)(C)(B)(A)";
I up(I l,S s) { I p=0; DO(l,p+=(s[i]=='(')-(s[i]==')');P(p<0,i)); R -1; }
#define prog(i) I _j=i; DO(l,s[l-i-1+pl]=cs[_j%8];_j/=8); I u=up(l,s+pl); if (u>=0) { i+=ipow(8,l-u-1)-1; continue; }
I oflen(I l) { I pl=strlen(prelude); S s=MA(l+pl+1); strcpy(s,prelude); s[l+pl]=0; T ast; D r=newRoot();
	DO(ipow(8,l), prog(i); parses(s,&ast)&&small(&ast,r)&&comb(ast)&&PF("%s|",s)&&pr(ast); freeTerms(ast)); freeRules(r); FR(s); }
I main(I argc,S argv[]){ P(argc==1,puts("usage: ./bible <length>")); DO(1+atoi(argv[1]),oflen(i)); }
