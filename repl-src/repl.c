#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include "prelude.c"
#include "../mlatu.h"
#include "../mlatuMacros.h"

I dbg=0, tmr=0, cnt=0;
V prD(D d,I i) { DO(i,PF("  ")); PF(" %s: ",d->w);
	if (d->r) { C s=prettyTerms(d->r); PF("%s",s); fr(s); } else if (d->e) PF("[empty rewrite]");
	PF("\n"); MAP(d->c,prD(c,i+1)); }
V sys(C s,D root) { C t=strtok(s," ");
	if (!strcmp(t,")h")&&!strtok(0,s)) PF(
		" )h         you are here\n"
		" bye        exit\n"
		" )d         toggle debug mode\n"
		" )t         toggle timer mode\n"
		" )c         toggle counting mode\n"
		" )ruletree  print tree of all user-defined rules\n");
	else if (!strcmp(t,")d")&&!strtok(0,s)) PF(" turning debug mode %s\n",   (dbg=!dbg)?"on":"off");
	else if (!strcmp(t,")t")&&!strtok(0,s)) PF(" turning timer mode %s\n",   (tmr=!tmr)?"on":"off");
	else if (!strcmp(t,")c")&&!strtok(0,s)) PF(" turning counting mode %s\n",(cnt=!cnt)?"on":"off");
	else if (!strcmp(t,")ruletree")&&!strtok(0,s)) {MAP(root->c,prD(c,0));}
	else PF(" invalid command\n"); }
V pT(I ms) { I h=ms/3600000, m=(ms-h*3600000)/60000, s=(ms-m*60000-h*3600000)/1000; ms%=1000;
	switch (h?0:m?1:s?2:3) { case 0: PF("%dh ",h); case 1: PF("%dm ",m); case 2: PF("%ds ",s); case 3: PF("%dms",ms); } }

V pra(T ast) { C s=prettyTerms(ast); PF("|-> %s\n",s); fr(s); }
#define e(er,n) if (er) { switch(er) {                                                              \
	case OPEN:  PF("error opening file '%s'\n",n); B;                                               \
	case PRN:   PF("error parsing file '%s': unbalanced parentheses\n",n); B;                       \
	case PRD:   PF("error parsing file '%s': exactly one period expected in each rule\n",n); B;     \
	case EQ:    PF("error parsing file '%s': exactly one equal sign expected in each rule\n",n); B; \
	case EMPTY: PF("error parsing file '%s': cannot match with empty LHS\n",n); B;                  \
	case END:   PF("error parsing file '%s': period expected at end of every rule\n",n); B;         \
	case MCH:   PF("error parsing file '%s': quotes are opaque and cannot be matched on\n",n); B; } \
	freeRules(root); exit(-1); }
I main(I ac,C *av) { char s[999]; D root=newRoot(); I er=parseRules(prelude,root); e(er,"prelude"); term ast;
	DO(ac-1,I er=parseFile(av[i+1],root);e(er,av[i+1]);); // files
	PF(" readable-mlatu repl - github.com/mlatu-lang/readable-mlatu\n bye to exit, )h for help\n");
	Time st, pr, fn; I ms, sc, m, h;
	while (fgets(s,100,stdin)) { s[strlen(s)-1]=0; if (*s==')') { sys(s,root); continue; } if (tmr) getTime(&st);
		ast=parseTerms(s,&er); if (er) { switch (er) { 
			case PRN:   PF("X-> parsing error: unbalanced parentheses\n"); B;
			case EQ:    PF("X-> parsing error: equal sign\n\
    if you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file\n"); B;
			case PRD:  PF("X-> parsing error: period\n\
    if you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file\n"); B;
		} freeTerms(ast); continue; }
		I show=1, n=0; if (dbg) { while (!stepRewrite(root,ast)) { n++; show=0; pra(ast); } } else n=rewrite(root,ast); 
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { freeTerms(ast); B; };
		if (show) { getTime(&pr); pra(ast); } if (cnt) PF(" took %d rewrite%s\n", n, n==1?"":"s");
		if (tmr) { getTime(&fn); ms=milliDiff(&st,&fn); PF(" "); pT(ms);
			if (!dbg) { ms=milliDiff(&pr,&fn); PF(" ("); pT(ms); PF(" outputting)"); } PF("\n"); }
		freeTerms(ast); }
	freeRules(root); }
