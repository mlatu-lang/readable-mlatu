#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include "prelude.c"
#include "../mlatu.h"
#include "../mlatuMacros.h"

I dbg, tmr, cnt;
V prD(D d,I i) { DO(i,PF("  ")); PF(" %s: ",d->w);
	if (d->r) { S s=prettyTerms(d->r); PF("%s",s); fr(s); } else if (d->e) PF("[empty rewrite]");
	PF("\n"); MAP(d->c,prD(c,i+1)); }
I fI, fL=100; S *f; V aF(S s) { S n=malloc(strlen(s)+1); strcpy(n,s); if (fI==fL) f=realloc(f,fL+=100); f[fI++]=n; } // add file
void e(I er,S n,I ex,D root) { if (er) { if (!ex) PF(" "); switch(er) {
	case OPEN:  PF("error opening file '%s'\n",n); B;
	case PRN:   PF("error parsing file '%s': unbalanced parentheses\n",n); B;
	case PRD:   PF("error parsing file '%s': exactly one period expected in each rule\n",n); B;
	case EQ:    PF("error parsing file '%s': exactly one equal sign expected in each rule\n",n); B;
	case EMPTY: PF("error parsing file '%s': cannot match with empty LHS\n",n); B;
	case END:   PF("error parsing file '%s': period expected at end of every rule\n",n); B;
	case MCH:   PF("error parsing file '%s': quotes are opaque and cannot be matched on\n",n); B; }
	if (ex) { DO(fI,fr(f[i])); freeRules(root), fr(f), exit(-1); } } }
V sys(S s,D root) { S t=strtok(s," "); S n=strtok(0," ");
	if (!strcmp(t,")h")&&!n) PF(
		" )h             you are here\n"
		" bye            exit\n"
		" )d             toggle debug mode\n"
		" )t             toggle timer mode\n"
		" )c             toggle counting mode\n"
		" )ruletree      print tree of all user-defined rules\n"
		" )rule <terms>  print all rules starting with <terms>\n"
		" #wield <file>  load ruleset from file\n"
		" )r             reload all loaded files\n");
	else if (!strcmp(t,")d")&&!n) PF(" turning debug mode %s\n",   (dbg=!dbg)?"on":"off");
	else if (!strcmp(t,")t")&&!n) PF(" turning timer mode %s\n",   (tmr=!tmr)?"on":"off");
	else if (!strcmp(t,")c")&&!n) PF(" turning counting mode %s\n",(cnt=!cnt)?"on":"off");
	else if (!strcmp(t,")ruletree")&&!n) {MAP(root->c,prD(c,0));}
	else if (!strcmp(t,"#wield")&&n) { I er=parseFile(n,root); e(er,n,0,0); aF(n); }
	else if (!strcmp(t,")r")&&!n) { DO(fI,parseFile(f[i],root)); }
	else PF(" invalid command\n"); }
V pT(I ms) { I h=ms/3600000, m=(ms-h*3600000)/60000, s=(ms-m*60000-h*3600000)/1000; ms%=1000;
	switch (h?0:m?1:s?2:3) { case 0: PF("%dh ",h); case 1: PF("%dm ",m); case 2: PF("%ds ",s); case 3: PF("%dms",ms); } }

V O(T ast) { S s=prettyTerms(ast); PF("|-> %s\n",s); fr(s); }
I main() { D root=newRoot(); I er=parseRules(prelude,root); e(er,"prelude",1,root); f=malloc(fL);
	PF(" readable-mlatu repl - github.com/mlatu-lang/readable-mlatu\n bye to exit, )h for help\n");
	char s[999]; T ast; Time st, pr, fn; I ms, sc, m, h;
	while (fgets(s,999,stdin)) { s[strlen(s)-1]=0; if (*s==')'||!strncmp(s,"#wield ",7)) { sys(s,root); continue; }
		if (tmr) getTime(&st); S i=strchr(s,'#'); if (i) *i=0; ast=parseTerms(s,&er); if (er) { switch (er) {
			case PRN: PF("X-> parsing error: unbalanced parentheses\n"); B;
			case EQ:  PF("X-> parsing error: equal sign\n\
    if you are trying to define a rule, it cannot be defined in the repl, you need to load it from a file\n"); B;
			case PRD: PF("X-> parsing error: period\n\
    if you are trying to define a rule, it cannot be defined in the repl, you need to load it from a file\n"); B;
		} freeTerms(ast); continue; }
		I show=1, n=0; if (dbg) { while (!stepRewrite(root,ast)) { n++; show=0; O(ast); } } else n=rewrite(root,ast);
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { freeTerms(ast); B; };
		if (show) { getTime(&pr); O(ast); } if (cnt) PF(" took %d rewrite%s\n", n, n==1?"":"s");
		if (tmr) { getTime(&fn); ms=milliDiff(&st,&fn); PF(" "); pT(ms);
			if (!dbg) { ms=milliDiff(&pr,&fn); PF(" ("); pT(ms); PF(" outputting)"); } PF("\n"); }
		freeTerms(ast); }
	freeRules(root); DO(fI,fr(f[i])); fr(f); }
