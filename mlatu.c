#include <math.h>
#include "c.h"
#include "p.c"
V rm(M m) { M n=m->n->n; fM(m->n); m->n=n; }
I wP(M M) { R 1; } I qP(M m) { R m->t==Q; } // ?w and ?q
V rPtv(M m) { rm(m); rm(m); } 
V sPtv(M m) { M c=m->n; m->n=c->n; c->n=m->n->n; m->n->n=c; rm(m->n->n); }
V dPtv(M m) { rm(m->n); M c=cM(m->n); c->n=m->n->n; m->n->n=c; }
V qPtv(M m) { rm(m->n); M q=nM(Q,""); q->c=m->n; q->n=m->n->n; q->c->n=0; m->n=q; }
V uPtv(M m) { M cs=m->n->c; m->n->c=0; rm(m); rm(m); if (!cs) R; MAP(cs,); c->n=m->n; m->n=cs;  }
V cPtv(M m) { M q=m->n->n; MAP(m->n->c,); c?(c->n=q->c):(m->n->c=q->c); q->c=0; rm(m->n); rm(m->n); }
V lenPtv(M m) { I i=0; MAP(m->n->c,); C w=m->n->n->w; w=realloc(w,i?floor(log10(i))+2:2); sprintf(w,"%d",i); }
 
I lst; V prML(M m); V prM(M m) { lst==Q?lst=TRM:PF(" "); switch (m->t) { // print ast node
	case TRM: DO(strlen(m->w),I e=esc(m->w[i]); PF("%*s%c",e,e?"`":"",m->w[i])); break;
	case Q: PF("("); lst=Q; prML(m->c); PF(")"); lst=TRM; break; } }
V prML(M m) { MAP(m,prM(c)); } // prints full list of M 
V prAST(M m) { lst=TRM; PF("|->"); prML(m->n); PF("\n"); } // print ast

I dbg=0, ch;
typedef struct ll { D d; struct ll *n; } *LL; I sLL=sizeof(struct ll);
M idx(M oM,I t) { I i=0; M m=oM; while (m->n&&i++<t) m=m->n; R m; } // index LL
V mch(M m,D r,LL ms) /* finds first match in m */ { if (!m) R; D cR=r; while (cR) { //PF("(%s %s) ",m->w,cR->w);
	if (cR->p) { if (!(cR->p(m))) R; } else if (strcmp(m->w,cR->w)) R; LL lst; MAP(ms,); lst=c;
	if (!(cR->c)) { LL n=ma(sLL); n->d=cR; n->n=0; lst->n=n; R; } {MAP(cR->c,mch(m->n,c,ms))} cR=cR->n; } }
V ex(M m,D r) /* rewrites */ { I i=0, l; LL ms=ma(sLL); ms->n=0; while (1) { l=0; {MAP(m->n,l++)} if (i>=l) break;
	M iM=idx(m,i); ms->n=0; MAP(ms->n,fr(c)); mch(iM->n,r,ms); i++;
	if (ms->n) { ch=1; LL r=ms->n; MAP(ms->n,r=c->d->l>r->d->l?c:r); 
		if (r->d->f) r->d->f(iM); else { DO(r->d->l+1,rm(iM)); M z=cM(r->d->r); MAP(z,); c->n=iM->n; iM->n=z; }
		i=0; if (dbg) prAST(m); } } }
I main(I ac,C *av) { D a=nID(wP,0,"?a"), r=nD("r",rPtv), d=nD("d",dPtv), q=nD("q",qPtv), len=nD("len",lenPtv),
		a2=nID(wP,0,"?a"), s=nD("s",sPtv),
		q1=nID(qP,0,"?q"), u=nD("u",uPtv), q2=nID(qP,0,"q"), c=nD("c",cPtv);
	nC(a,r); nC(a,d); nC(a,q); nC(a,a2); nC(a2,s); a->n=q1; nC(q1,u); nC(q1,len); nC(q1,q2); nC(q2,c); // set ptvs
	DO(ac-1,if (!strcmp("-d",av[i+1])) dbg=1; else { FILE *f=fopen(av[i+1],"r"); 
		if (!f) { PF("error opening file %s\n",av[i+1]); goto exit; } pF(f); }); // flags and rules
	C t=ma(100); I i, l;
	while (gets(t)) { l=ch=i=0; M ast=nM(ST,""); P(t,&i,&ast->n); // repl
		if (i!=1+strlen(t)) { PF("X-> parsing error: mismatched parentheses\n"); goto end; }
		ex(ast,a); MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { fML(ast); exit(0); } 
		if (!dbg||!ch) prAST(ast); end: fML(ast); }
	exit: fD(a); }
