#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuMacros.h"
#include "mlatu.h"
V pr(T t) { PF("|-> "); prettyTerms(t); PF("\n"); }
V f(C n,rule root) {
	I er=parseFile(n,root); if (er) { switch(er) {
		case OPEN: PF("Error opening file '%s'",n); B;
		case PRN: PF("Error parsing file '%s': unbalanced parentheses",n); B;
		case UNESC: PF("Error parsing file '%s': unescaped backtick",n); B;
		case SEMI: PF("Error parsing file '%s': exactly one semicolon expected in each rule",n); B;
		case EQ: PF("Error parsing file '%s': exactly one equal sign expected in each rule",n); B;
		case EMPTY: PF("Error parsing file '%s': cannot match with empty LHS",n); B;
		case END: PF("Error parsing file '%s': semicolon expected at end of every rule",n); B;
		case MCH: PF("Error parsing file '%s': quotes are opaque and cannot be matched on",n); B; } exit(-1); } }
I main(I ac,C *av) { I dbg=0; C s=ma(100); rule root=newRoot(); f("prelude.mlt",root); term ast;
	DO(ac-1,if(!strcmp("-d",av[i+1]))dbg=1;else f(av[i+1],root)); // flags and files
	while (fgets(s,100,stdin)) { s[strlen(s)-1]='\0'; I er; ast=parseTerms(s,&er);
		if (er) { switch (er) { 
			case PRN:   PF("X-> parsing error: unbalanced parentheses\n"); B;
			case UNESC: PF("X-> parsing error: unescaped backtick\n"); B;
			case EQ:    PF("X-> parsing error: unescaped equal sign. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
			case SEMI:  PF("X-> parsing error: unescaped semicolon. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
		} goto end; }
		I ch=0; if (dbg) { while (!stepRewrite(root,ast)) { ch=1; pr(ast); } } else rewrite(root,ast); 
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) B;
		if (!ch) pr(ast); end: freeTerms(ast); } freeRules(root); fr(s); }
