#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include "prelude.c"
#include "../mlatu.h"
#include "../mlatuHelpers.c"

D root; T path; I sil, dbg, tmr, cnt;
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
T lf; /* uniq */ V rF(S s) /* rm */ { MAP(lf,P(_n&&SQ(_n->w,s),(c->n=_n->n,_n->n=0,freeTerms(_n)))); }
V aF(S n) /* add */ { rF(n); MAP(lf,); c->n=newTerm(0,n); }
V lF(S n) /* load */ { SQ(n,"PRELUDE")?parseRules(PRLD,root,path):fE(parseFile(n,root,path)); }
V O(T t,S f) { S s=prettyTerms(t); PF(f,s); FR(s); }
V prD(D d,I i) { DO(i,PF(" ")); PF("%s: ",d->w);
	if (d->r) O(d->r,"%s"); else if (d->e) PF("[empty rewrite]"); PF("\n"); MAP(d->c,prD(c,i+1)); }
V pRH(S s,D d) /* )rule helper */ { if (d->e||d->r) { PF(" %s =",s); if (d->r) O(d->r," %s"); PF(" .\n"); }
	MAP(d->c,S t=MA(strlen(s)+strlen(c->w)+2); strcpy(t,s); strcat(t," "); strcat(t,c->w); pRH(t,c); FR(t)); }
V pR(S s,D d) { T t; E e=parseTerms(s+6,&t); P(e.e,freeTerms(t),pE(e,0));
	MAP(t->n,P(!(d=fnd(d->c,c->w)),)); S u=prettyTerms(t); freeTerms(t); pRH(u,d); FR(u); }
V sys(S oS) { S s; SC(s,oS); S t=strtok(s," \n"); S n=strtok(0," \n");
	if      (SQ(t,"#wield")   &&n) aF(n), lF(n);
	else if (SQ(t,"#unwield") &&n) { rF(n); goto rel; }
	else if (SQ(t,")r")       &&!n) { rel: freeRules(root); root=newRoot(); MAP(lf->n,lF(c->w)); }
	else if (SQ(t,")lf")      &&!n) { MAP(lf->n,PF(" %s\n",c->w)); }
	else if (SQ(t,")d")       &&!n) sil||PF(" turning debug mode %s\n",   (dbg=!dbg)?"on":"off");
	else if (SQ(t,")t")       &&!n) sil||PF(" turning timer mode %s\n",   (tmr=!tmr)?"on":"off");
	else if (SQ(t,")c")       &&!n) sil||PF(" turning counting mode %s\n",(cnt=!cnt)?"on":"off");
	else if (SQ(t,")ruletree")&&!n) {MAP(root->c,prD(c,1));}
	else if (SQ(t,")rule")) pR(oS,root);
	else if (SQ(t,")h")) {
		if (!n) PF(
			" )h               you are here\n"
			"  )h path         how to set a path to search when wielding files\n"
			" bye              exit\n"
			" ^                special term that rewrites to last result\n"
			" #wield   <file>  load ruleset from file\n"
			" #unwield <file>  unload a file\n"
			" )r               reload all loaded files\n"
			" )lf              print the list of loaded files\n"
			" )d               toggle debug mode\n"
			" )t               toggle timer mode\n"
			" )c               toggle counting mode\n"
			" )rule <terms>    print all rules starting with <terms>\n"
			" )ruletree        print tree of all user-defined rules (like  )rule  but nicer)\n");
		else if (SQ(n,"path")) PF(
			" if it exists, the file ~/.mlatu_path is read when the repl is started\n"
			" each line should be a path to search when wielding files\n");
		else PF(" invalid command\n"); }
	else PF(" invalid command\n"); }
V pT(I ms) { I h=ms/3600000, m=(ms-h*3600000)/60000, s=(ms-m*60000-h*3600000)/1000; ms%=1000;
	switch (h?0:m?1:s?2:3) { C 0: PF("%dh ",h); C 1: PF("%dm ",m); C 2: PF("%ds ",s); C 3: PF("%dms",ms); } }

S xp(S s) /* expand */ { wordexp_t x; wordexp(s,&x,0); S z; SC(z,*x.we_wordv); wordfree(&x); R z; }
T gP() /* get path */ { S d=xp("~/.mlatu_path"); FILE *f=fopen(d,"rb"); FR(d); P(!f,0); T z=newTerm(0,""), c=z;
	while (1) { S l=0, s, e; size_t m; if (getline(&l,&m,f)==-1) { FR(l); B; }
		s=l; while (*s&&WS(*s)) s++; e=l+strlen(l)-1; while (WS(*e)&&e>=s) *e=0, e--; S x=xp(s); FR(l); c=c->n=newTerm(0,x); FR(x);
	} R z; }

I main(I ac,S *av) { DO(ac-1,if (SQ("-s",av[i+1])) sil=1); lf=newTerm(0,"");
	root=newRoot(); path=gP(); aF("PRELUDE"); lF("PRELUDE"); char s[999]; T ast; Time st, pr, fn; I ms, sc, m, h;
	if (!sil) PF(" how may readable-mlatu ease your life, oh grand exalted master?\n bye to exit, )h for help\n");
	while (fgets(s,999,stdin)) { if (SQ("bye\n",s)) B; if (*s==')'||!strncmp(s,"#wield ",7)||!strncmp(s,"#unwield ",9)) { sys(s); continue; }
		rn(&st); S i=strchr(s,'#'); if (i) *i=0; E e=parseTerms(s,&ast); if (e.e) { pE(e,1); goto end; }
		I n=0; if (dbg) while (!stepRewrite(root,ast)) n++, sil||PF(" |-> "), O(ast,"%s\n"); else n=rewrite(root,ast);
		rn(&pr); S s=prettyTerms(ast); if (!dbg||!n) sil||PF(" |-> "), puts(s); if (cnt) PF(" %d rewrite%s\n", n, n==1?"":"s"); rn(&fn);
		if (tmr) { PF(" "); pT(msD(&st,dbg?&fn:&pr)); if (!dbg) PF(" rewriting, "), pT(msD(&pr,&fn)), PF(" printing"); PF("\n"); }
		S r=MA(strlen(s)+4); strcpy(r,"^="); strcat(r,s); FR(s); strcat(r,"."); parseRules(r,root,path); FR(r); end: freeTerms(ast); }
	freeRules(root); freeTerms(path); freeTerms(lf); }
