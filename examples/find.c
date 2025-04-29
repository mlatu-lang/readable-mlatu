/* find.c
   brute force combinators
   usage: ./find (<before> <after>)+
   example usage: ./find '(B)(A)' 'A' '(B)(A)' '(A(B))'
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
#define progs(i) I _j=i; DO(l,I k=i;DO(w,s[i][k+pls[i]]=cs[_j%8]);_j/=8)
I oflen(I l,I w,S*ls,T*rs) { PF("length %d:\n",l); S*s=MA(w*sizeof(S)); I*pls=MA(w*sizeof(I)); DO(w,pls[i]=strlen(ls[i]));
	DO(w,s[i]=MA(l+pls[i]+1);strcpy(s[i],ls[i]);s[i][l+pls[i]]=0); T ast; D r=newRoot();
	DO(ipow(8,l), progs(i);
		DO(w,parses(s[i],&ast)&&small(&ast,r)&&teq(ast,rs[i])&&PF("%s |-> ",s[i])&&pr(ast);freeTerms(ast)));
	freeRules(r); DO(w,FR(s[i])); FR(s); FR(pls); }
S help="usage: ./find (<before> <after>)*\nexample: ./find '(B)(A)' 'A' '(B)(A)' '(A(B))'";
I main(I argc,S argv[]) { P(argc==1||!(argc%2),puts(help)); I w=argc/2; S*ls=MA(w*sizeof(S)); T l,*rs=MA(w*sizeof(T));
	DO(w,parse(ls[i]=argv[2*i+1],&l);freeTerms(l);parse(argv[2*(i+1)],&rs[i])); DO(100,oflen(i,w,ls,rs));
	DO(w,freeTerms(rs[i])); FR(ls); FR(rs); }
