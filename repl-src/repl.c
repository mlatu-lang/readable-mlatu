#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prelude.c"
#include "../mlatu.h"
#include "../mlatuMacros.h"

I dbg=0;
V prD(D d,I i) { DO(i,PF("  ")); PF(" %s: ",d->q?"(?q)":d->w);
	if (d->r||d->f||d->e) { if (d->f) PF("[internal rewrite]"); if (d->e) PF("[empty rewrite]");
		if (d->r) { C s=prettyTerms(d->r); PF("%s",s); fr(s); } }
	PF("\n"); MAP(d->c,prD(c,i+1)); }
V sys(C s,D root) { C t=strtok(s," ");
	if (!strcmp(t,")h")&&!strtok(0,s)) PF(
		" )h         you are here\n"
		" bye        exit\n"
		" )d         toggle debug mode\n"
		" )ruletree  print tree of all defined rules\n"
		" the term (?q) is used to represent an internal rule that matches on any quote\n"); 
	else if (!strcmp(t,")d")&&!strtok(0,s)) PF(" Turning debug mode %s\n",(dbg=!dbg)?"on":"off");
	else if (!strcmp(t,")ruletree")&&!strtok(0,s)) {MAP(root->c,prD(c,0));}
	else PF(" invalid command\n"); }

V pr(T ast) { C s=prettyTerms(ast); PF("|-> %s\n",s); fr(s); }
V e(I er,C n) { if (er) { switch(er) {
	case OPEN:  PF("Error opening file '%s'\n",n); B;
	case PRN:   PF("Error parsing file '%s': unbalanced parentheses\n",n); B;
	case SEMI:  PF("Error parsing file '%s': exactly one semicolon expected in each rule\n",n); B;
	case EQ:    PF("Error parsing file '%s': exactly one equal sign expected in each rule\n",n); B;
	case EMPTY: PF("Error parsing file '%s': cannot match with empty LHS\n",n); B;
	case END:   PF("Error parsing file '%s': semicolon expected at end of every rule\n",n); B;
	case MCH:   PF("Error parsing file '%s': quotes are opaque and cannot be matched on\n",n); B; } exit(-1); } }
I main(I ac,C *av) { C s=ma(100); D root=newRoot(); I er=parseRules(prelude,root); e(er,"prelude"); term ast;
	DO(ac-1,I er=parseFile(av[i+1],root);e(er,av[i+1]);); // files
	PF(" readable-mlatu repl - github.com/mlatu-lang/readable-mlatu\n bye to exit, )h for help\n");
	while (fgets(s,100,stdin)) { s[strlen(s)-1]=0; if (*s==')') { sys(s,root); continue; } I er; ast=parseTerms(s,&er);
		if (er) { switch (er) { 
			case PRN:   PF("X-> parsing error: unbalanced parentheses\n"); B;
			case EQ:    PF("X-> parsing error: equal sign. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
			case SEMI:  PF("X-> parsing error: semicolon. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
		} goto end; }
		I show=1; if (dbg) { while (!stepRewrite(root,ast)) { show=0; pr(ast); } } else rewrite(root,ast); 
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) B;
		if (show) pr(ast); end: freeTerms(ast); } freeRules(root); fr(s); }
