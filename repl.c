#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuMacros.h"
#include "mlatu.h"
I dbg=0;
V pr(term t) { PF("|-> "); prettyTerms(t); PF("\n"); }
I main(I ac,C *av) { C s=ma(100); rule root=newRoot(); pF("prelude.mlt",root); /*prettyRules(root);*/ term ast;
	DO(ac-1,if(!strcmp("-d",av[i+1]))dbg=1;else{C n=av[i+1];I er=pF(n,root);if(er){switch(er){ // flags and files
		case OPEN: PF("Error opening file '%s'",n); exit(-1);
		case PRN: PF("Error parsing file '%s': unbalanced parentheses",n); exit(-1);
		case UNESC: PF("Error parsing file '%s': unescaped backtick",n); exit(-1);
		case SEMI: PF("Error parsing file '%s': exactly one semicolon expected in each rule",n); exit(-1);
		case EQ: PF("Error parsing file '%s': exactly one equal sign expected in each rule",n); exit(-1);
		case EMPTY: PF("Error parsing file '%s': cannot match with empty LHS",n); exit(-1);
		case END: PF("Error parsing file '%s': semicolon expect at end of every rule",n); exit(-1);
		case MCH: PF("Error parsing file '%s': quotes are opaque and cannot be matched on",n); exit(-1); }}})
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
