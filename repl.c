#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prelude.c"
#include "mlatuMacros.h"
V pr(T t) { PF("|-> "); prettyTerms(t); PF("\n"); }
V e(I er,C n) { if (er) { switch(er) {
	case OPEN:  PF("Error opening file '%s'\n",n); B;
	case PRN:   PF("Error parsing file '%s': unbalanced parentheses\n",n); B;
	case SEMI:  PF("Error parsing file '%s': exactly one semicolon expected in each rule\n",n); B;
	case EQ:    PF("Error parsing file '%s': exactly one equal sign expected in each rule\n",n); B;
	case EMPTY: PF("Error parsing file '%s': cannot match with empty LHS\n",n); B;
	case END:   PF("Error parsing file '%s': semicolon expected at end of every rule\n",n); B;
	case MCH:   PF("Error parsing file '%s': quotes are opaque and cannot be matched on\n",n); B; } exit(-1); } }
I main(I ac,C *av) { I dbg=0; C s=ma(100), n; rule root=newRoot(); I er=parseRules(prelude,root); e(er,"prelude"); term ast;
	DO(ac-1,if(!strcmp("-d",n=av[i+1]))dbg=1;else{I er=parseFile(n,root);e(er,n);}); // flags and files
	while (fgets(s,100,stdin)) { s[strlen(s)-1]='\0'; I er; ast=parseTerms(s,&er);
		if (er) { switch (er) { 
			case PRN:   PF("X-> parsing error: unbalanced parentheses\n"); B;
			case EQ:    PF("X-> parsing error: equal sign. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
			case SEMI:  PF("X-> parsing error: semicolon. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
		} goto end; }
		I ch=0; if (dbg) { while (!stepRewrite(root,ast)) { ch=1; pr(ast); } } else rewrite(root,ast); 
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) B;
		if (!ch) pr(ast); end: freeTerms(ast); } freeRules(root); fr(s); }
