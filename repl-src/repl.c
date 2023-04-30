#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include "prelude.c"
#include "../mlatu.h"
#include "../mlatuMacros.h"

I dbg, tmr, cnt;
V prD(D d,I i) { DO(i,PF(" ")); PF("%s: ",d->w);
	if (d->r) { S s=prettyTerms(d->r); PF("%s",s); fr(s); } else if (d->e) PF("[empty rewrite]");
	PF("\n"); MAP(d->c,prD(c,i+1)); }
I fI, fL=100; S *f; V aF(S s) /* add file */ { S n; sc(n,s); if (fI==fL) f=realloc(f,fL+=100); f[fI++]=n; }
V fE(E e,D root) { P(!e.e); PF(" error %sing file '%s'",e.e==OPEN?"open":"pars",e.f); fr(e.f); switch (e.e) {
	C OPEN:  PF("\n"); B;
	C PRN:   PF(": unbalanced parentheses\n"); B;
	C EQ:    PF(": exactly one equal sign expected in each rule\n"); B;
	C EMPTY: PF(": cannot match with empty LHS\n"); B;
	C PRD:   PF(": period expected at end of every rule\n"); B;
	C MCH:   PF(": quotes are opaque and cannot be matched on\n"); B; } }
V pE(I er,I v) { PF("X -> parsing error: "); switch (er) {
	C PRN:   PF("unbalanced parentheses\n"); B;
	C EQ:    PF("equal sign\n");
		if (v)  PF("if you are trying to define a rule, it cannot be defined in the repl, you need to load it from a file\n"); B;
	C PRD:   PF("period\n");
		if (v)  PF("if you are trying to define a rule, it cannot be defined in the repl, you need to load it from a file\n"); B; } }
V O(T t,S f) { S s=prettyTerms(t); PF(f,s); fr(s); }
V pRH(S s,D d) /* )rule helper */ { if (d->e||d->r) { PF(" %s =",s); if (d->r) O(d->r," %s"); PF(" .\n"); }
	MAP(d->c,S t=ma(strlen(s)+strlen(c->w)+2); strcpy(t,s); strcat(t," "); strcat(t,c->w); pRH(t,c); fr(t)); }
V pR(S s,D root) { I er=0; T t=parseTerms(s+6,&er); P(er,pE(er,0)); D d=root;
	MAP(t->n,T b=c; {MAP(d->c,if (!strcmp(b->w,c->w)) goto e) R; e: d=c; }); S u=prettyTerms(t); freeTerms(t); pRH(u,d); fr(u); }
V sys(S oS,D root) { S s; sc(s,oS); S t=strtok(s," "); S n=strtok(0," ");
	if (!strcmp(t,")h")&&!n) PF(
		" )h             you are here\n"
		" bye            exit\n"
		" #wield <file>  load ruleset from file\n"
		" )r             reload all loaded files\n"
		" )d             toggle debug mode\n"
		" )t             toggle timer mode\n"
		" )c             toggle counting mode\n"
		" )rule <terms>  print all rules starting with <terms>\n"
		" )ruletree      print tree of all user-defined rules (like  )rule  but nicer)\n");
	else if (!strcmp(t,"#wield")&&n) { E er=parseFile(n,root); fE(er,0); aF(n); }
	else if (!strcmp(t,")r")&&!n) DO(fI,E er=parseFile(f[i],root); fE(er,0))
	else if (!strcmp(t,")d")&&!n) PF(" turning debug mode %s\n",   (dbg=!dbg)?"on":"off");
	else if (!strcmp(t,")t")&&!n) PF(" turning timer mode %s\n",   (tmr=!tmr)?"on":"off");
	else if (!strcmp(t,")c")&&!n) PF(" turning counting mode %s\n",(cnt=!cnt)?"on":"off");
	else if (!strcmp(t,")ruletree")&&!n) { MAP(root->c,prD(c,1)); }
	else if (!strcmp(t,")rule")) pR(oS,root);
	else PF(" invalid command\n"); }
V pT(I ms) { I h=ms/3600000, m=(ms-h*3600000)/60000, s=(ms-m*60000-h*3600000)/1000; ms%=1000;
	switch (h?0:m?1:s?2:3) { case 0: PF("%dh ",h); case 1: PF("%dm ",m); case 2: PF("%ds ",s); case 3: PF("%dms",ms); } }

I main() { D root=newRoot(); parseRules(prelude,root); f=malloc(fL);
	PF(" how may readable-mlatu ease your life, oh grand exalted master?\n bye to exit, )h for help\n");
	char s[999]; T ast; Time st, pr, fn; I ms, sc, m, h;
	while (fgets(s,999,stdin)) { s[strlen(s)-1]=0; if (*s==')'||!strncmp(s,"#wield ",7)) { sys(s,root); continue; }
		if (tmr) getTime(&st); S i=strchr(s,'#'); if (i) *i=0; I todoerr; ast=parseTerms(s,&todoerr); if (todoerr) { pE(todoerr,1); freeTerms(ast); continue; }
		I show=1, n=0; if (dbg) { while (!stepRewrite(root,ast)) { n++; show=0; O(ast,"|-> %s\n"); } } else n=rewrite(root,ast);
		I l=0; MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { freeTerms(ast); B; };
		if (show) { getTime(&pr); O(ast,"|-> %s\n"); } if (cnt) PF(" took %d rewrite%s\n", n, n==1?"":"s");
		if (tmr) { getTime(&fn); ms=milliDiff(&st,&fn); PF(" "); pT(ms);
			if (!dbg) { ms=milliDiff(&pr,&fn); PF(" ("); pT(ms); PF(" printing)"); } PF("\n"); }
		freeTerms(ast); }
	freeRules(root); DO(fI,fr(f[i])); fr(f); }
