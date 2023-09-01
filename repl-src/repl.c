#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include "prelude.c"
#include "../mlatu.h"
#include "../mlatuHelpers.c"

T path; I sil, dbg, tmr, cnt;
I fI, fL; S *f; V aF(S s) /* add file */ { DO(fI,P(SQ(s,f[i]),)); S n; SC(n,s); if (fI==fL) f=realloc(f,fL+=100); f[fI++]=n; }
V fE(E e) { P(!e.e); PF(" error %sing file '%s'",e.e==OPEN?"open":"pars",e.f); FR(e.f); if (e.e==OPEN) { PF("\n"); R; }
	PF(" on line %d: ",e.ln); switch (e.e) {
		C PRN:   PF("unbalanced parentheses\n"); B;
		C EQ:    PF("exactly one equal sign expected in each rule\n"); B;
		C EMPTY: PF("cannot match with empty LHS\n"); B;
		C PRD:   PF("period expected at end of every rule\n"); B;
		C MCH:   PF("quotes are opaque and cannot be matched on\n"); B; } }
V pE(E e,I v) { FR(e.f); sil||PF("X-> "); PF("parsing error: "); switch (e.e) {
		C PRN:   PF("unbalanced parentheses\n"); B;
		C EQ:    PF("equal sign\n"); B;
		C PRD:   PF("period\n"); B; }
	if (v) PF("if you are trying to define a rule, it cannot be defined in the repl; you need to load it from a file\n"); }
V O(T t,S f) { S s=prettyTerms(t); PF(f,s); FR(s); }
V prD(D d,I i) { DO(i,PF(" ")); PF("%s: ",d->w);
	if (d->r) O(d->r,"%s"); else if (d->e) PF("[empty rewrite]"); PF("\n"); MAP(d->c,prD(c,i+1)); }
V pRH(S s,D d) /* )rule helper */ { if (d->e||d->r) { PF(" %s =",s); if (d->r) O(d->r," %s"); PF(" .\n"); }
	MAP(d->c,S t=MA(strlen(s)+strlen(c->w)+2); strcpy(t,s); strcat(t," "); strcat(t,c->w); pRH(t,c); FR(t)); }
V pR(S s,D d) { T t; E e=parseTerms(s+6,&t); P(e.e,freeTerms(t),pE(e,0));
	MAP(t->n,P(!(d=fnd(d->c,c->w)),)); S u=prettyTerms(t); freeTerms(t); pRH(u,d); FR(u); }
V sys(S oS,D root) { S s; SC(s,oS); S t=strtok(s," \n"); S n=strtok(0," \n");
	if   (SQ(t,"#wield")&&n) fE(parseFile(n,root,aF,path)), aF(n);
	else if (SQ(t,")r")&&!n) DO(fI,fE(parseFile(f[i],root,aF,path)));
	else if (SQ(t,")d")&&!n) dbg=!dbg, sil||PF(" turning debug mode %s\n",   dbg?"on":"off");
	else if (SQ(t,")t")&&!n) tmr=!tmr, sil||PF(" turning timer mode %s\n",   tmr?"on":"off");
	else if (SQ(t,")c")&&!n) cnt=!cnt, sil||PF(" turning counting mode %s\n",cnt?"on":"off");
	else if (SQ(t,")rule")) pR(oS,root);
	else if (SQ(t,")ruletree")&&!n) {MAP(root->c,prD(c,1));}
	else if (SQ(t,")h")) {
		if (!n) PF(
			" )h             you are here\n"
			"  )h path       how to set a path to search when wielding files\n"
			" bye            exit\n"
			" #wield <file>  load ruleset from file\n"
			" ^              special term that rewrites to last result\n"
			" )r             reload all loaded files\n"
			" )d             toggle debug mode\n"
			" )t             toggle timer mode\n"
			" )c             toggle counting mode\n"
			" )rule <terms>  print all rules starting with <terms>\n"
			" )ruletree      print tree of all user-defined rules (like  )rule  but nicer)\n");
		else if (SQ(n,"path")) PF(
			" if it exists, the file ~/.mlatu_path is read when the repl is started\n"
			" each line should be a path to search when wielding files\n");
		else PF(" invalid command\n"); }
	else PF(" invalid command\n"); }
V pT(I ms) { I h=ms/3600000, m=(ms-h*3600000)/60000, s=(ms-m*60000-h*3600000)/1000; ms%=1000;
	switch (h?0:m?1:s?2:3) { C 0: PF("%dh ",h); C 1: PF("%dm ",m); C 2: PF("%ds ",s); C 3: PF("%dms",ms); } }

S xp(S s) /* expand */ { wordexp_t x; wordexp(s,&x,0); S z; SC(z,*x.we_wordv); wordfree(&x); R z; }
T gP() { S d=xp("~/.mlatu_path"); FILE *f=fopen(d,"rb"); FR(d); P(!f,0); T z=newTerm(ST,"");
	while (1) { S l=0, s, e; size_t m; if (getline(&l,&m,f)==-1) { FR(l); B; }
		s=l; while (*s&&WS(*s)) s++; e=l+strlen(l)-1; while (WS(*e)&&e>=s) *e=0, e--; S x=xp(s); FR(l);
		MAP(z,); c->n=newTerm(TRM,x); FR(x);
	} R z; }

I main(I ac,S *av) { DO(ac-1,if (SQ("-s",av[i+1])) sil=1); D root=newRoot(); path=gP(); parseRules(prelude,root,aF,path);
	char s[999]; T ast; Time st, pr, fn; I ms, sc, m, h;
	if (!sil) PF(" how may readable-mlatu ease your life, oh grand exalted master?\n bye to exit, )h for help\n");
	while (fgets(s,999,stdin)) { if (SQ("bye\n",s)) B; if (*s==')'||!strncmp(s,"#wield ",7)) { sys(s,root); continue; }
		rn(&st); S i=strchr(s,'#'); if (i) *i=0; E e=parseTerms(s,&ast); if (e.e) { pE(e,1); goto end; }
		I n=0; if (dbg) while (!stepRewrite(root,ast)) n++, sil||PF(" |-> "), O(ast,"%s\n");  else n=rewrite(root,ast);
		rn(&pr); S s=prettyTerms(ast); if (!dbg||!n) sil||PF(" |-> "), puts(s); if (cnt) PF(" %d rewrite%s\n", n, n==1?"":"s"); rn(&fn);
		if (tmr) { PF(" "); pT(msD(&st,dbg?&fn:&pr)); if (!dbg) PF(" rewriting, "), pT(msD(&pr,&fn)), PF(" printing"); PF("\n"); }
		S r=MA(strlen(s)+4); strcpy(r,"^="); strcat(r,s); FR(s); strcat(r,"."); parseRules(r,root,aF,path); FR(r); end: freeTerms(ast); }
	freeRules(root); DO(fI,FR(f[i])); FR(f); }
