#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prelude.c"
#include "mlatuMacros.h"

I dbg=0;
V prD(D d,I i) { DO(i,PF("  ")); PF("%s: ",d->q?"?q":d->w);
	if (d->r||d->f||d->e) { if (d->f) PF("[internal rewrite]"); if (d->e) PF("[empty rewrite]");
		if (d->r) { C s=prettyTerms(d->r); PF("%s",s); fr(s); } }
	PF("\n"); MAP(d->c,prD(c,i+1)); }
I sys(T ast,D root) {
	if (!strcmp(ast->n->w,"]rules")&&!ast->n->n) {MAP(root->c,prD(c,0));}
	if (!strcmp(ast->n->w,"]d")&&!ast->n->n) PF("Turning debug mode %s\n",dbg?"off":"on"), dbg=!dbg;
	else R 1; R 0; }

V pr(T ast) { C s=prettyTerms(ast); PF("|-> %s\n",s); fr(s); }
V e(I er,C n) { if (er) { switch(er) {
	case OPEN:  PF("Error opening ruleset '%s'\n",n); B;
	case PRN:   PF("Error parsing ruleset '%s': unbalanced parentheses\n",n); B;
	case SEMI:  PF("Error parsing ruleset '%s': exactly one semicolon expected in each rule\n",n); B;
	case EQ:    PF("Error parsing ruleset '%s': exactly one equal sign expected in each rule\n",n); B;
	case EMPTY: PF("Error parsing ruleset '%s': cannot match with empty LHS\n",n); B;
	case END:   PF("Error parsing ruleset '%s': semicolon expected at end of every rule\n",n); B;
	case MCH:   PF("Error parsing ruleset '%s': quotes are opaque and cannot be matched on\n",n); B; } exit(-1); } }
I main(I ac,C *av) { C s=ma(100), n; D root=newRoot(); I er=parseRules(prelude,root); e(er,"prelude"); term ast;
	DO(ac-1,I er=parseFile(n,root);e(er,n);); // files
	while (fgets(s,100,stdin)) { s[strlen(s)-1]='\0'; I er; ast=parseTerms(s,&er);
		if (er) { switch (er) { 
			case PRN:   PF("X-> parsing error: unbalanced parentheses\n"); B;
			case EQ:    PF("X-> parsing error: equal sign. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
			case SEMI:  PF("X-> parsing error: semicolon. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
		} goto end; }
		I show=1; if (dbg) { while (!stepRewrite(root,ast)) { show=0; pr(ast); } } else rewrite(root,ast); 
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) B; if (*ast->n->w==']') show=sys(ast,root);
		if (show) pr(ast); end: freeTerms(ast); } freeRules(root); fr(s); }
