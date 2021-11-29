#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuMacros.h"
#include "mlatu.h"
I dbg=0;
V pr(term t) { PF("|-> "); prettyTerms(t); PF("\n"); }
I main(I ac,C *av) { C s=ma(100); rule root=newRoot(); pF("prelude.mlt",root); term ast;
	DO(ac-1,!strcmp("-d",av[i+1])?dbg=1:pF(av[i+1],root)) // flags and files
	while (fgets(s,100,stdin)) { s[strlen(s)-1]='\0'; I er; ast=parseTerms(s,&er);
		if (er) { switch (er) { 
			case PRN:          PF("X-> parsing error: unbalanced parentheses\n"); B;
			case UNESC:        PF("X-> parsing error: unescaped backtick\n"); B;
			case EQ: case QEQ: PF("X-> parsing error: unescaped equal sign. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
			case SEMI:         PF("X-> parsing error: unescaped semicolon. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
		} goto end; }
		I ch=0; if (dbg) { while (!stepRewrite(root,ast)) { ch=1; pr(ast); } } else rewrite(root,ast); 
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) B;
		if (!ch) pr(ast); end: freeTerms(ast); } freeTerms(ast); freeRules(root); fr(s); }
