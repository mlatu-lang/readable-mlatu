/* find.c
   brute force combinators
   usage: ./find <before> <after>
   example usage: ./find '(B)(A)'
   also see bible.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mlatu.h"
#include "../mlatuHelpers.c"
I parses(S s,T*ast) { E e=parseTerms(s,ast); FR(e.f); R !e.e; }
V parse(S s,T*ast) { if (!parses(s,ast)) puts("Invalid expression"), exit(1); }
I size(T ast) { I i=0; MAP(ast,i++;!c->w&&(i+=size(c->c))); R i; }
I small(T*ast,D r) { DO(1000,P(stepRewrite(r,ast),1);P(size(*ast)>1000,0)); R 0; }
I pr(T ast) { S s=prettyTerms(ast); puts(s); FR(s); }
I ipow(I b,I p) { I r=1; DO(p,r*=b); R r; }
S cs="()+-<>~,";
I teq(T t,T u) { MAP(t,P(!u||(c->w?!u->w||strcmp(c->w,u->w):u->w||!teq(c->c,u->c)),0);u=u->n); R !u; }
#define prog(i) I _j=i; DO(l,s[l-i-1+pl]=cs[_j%8];_j/=8)
I oflen(I l,S ls,T rs) { PF("length %d:\n",l); I pl=strlen(ls); S s=MA(l+pl+1); strcpy(s,ls); s[l+pl]=0; T ast; D r=newRoot();
	DO(ipow(8,l),prog(i);parses(s,&ast)&&small(&ast,r)&&teq(ast,rs)&&PF("%s |-> ",s)&&pr(ast);freeTerms(ast));
	freeRules(r); FR(s); }
S help="usage: ./find <before> <after>\nexample: ./find '(B)(A)' 'A'";
I main(I argc,S argv[]) { P(argc!=3,puts(help)); S ls=argv[1]; T rs; parse(ls,&rs); freeTerms(rs); parse(argv[2],&rs);
	DO(5,oflen(i,ls,rs)); freeTerms(rs); }
