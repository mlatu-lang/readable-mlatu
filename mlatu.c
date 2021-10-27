#include "c.h"
#include "p.c"
V r(M m) { M n=m->n->n; fM(m->n); m->n=n; }
I wP(M M) { R 1; } I qP(M m) { R m->t==Q; }
V rPtv(M m) { r(m); r(m); } 
V sPtv(M m) { M c=m->n; m->n=c->n; c->n=m->n->n; m->n->n=c; r(m->n->n); }
V dPtv(M m) { r(m->n); M c=cM(m->n); c->n=m->n->n; m->n->n=c; }
V qPtv(M m) { r(m->n); M q=nM(Q,""); q->c=m->n; q->n=m->n->n; q->c->n=0; m->n=q; }
V uPtv(M m) { M cs=m->n->c; m->n->c=0; r(m); r(m); if (!cs) R; MAP(cs); c->n=m->n; m->n=cs;  }
V cPtv(M m) { M q=m->n->n; MAP(m->n->c,); if (!c) m->n->c=q->c; else c->n=q->c; q->c=0; r(m->n); r(m->n); }
 
V prT(C t) { DO(strlen(t),printf("%*s%c",esc(t[i]),esc(t[i])?"`":"",t[i])); }
I lst; V prML(M m); V prM(M m) { if (lst==Q) lst=TRM; else PF(" "); switch (m->t) { 
	case TRM: prT(m->w); break; case Q: PF("("); lst=Q; prML(m->c); PF(")"); break; } }
V prML(M m) { MAP(m,prM(c)); } V prAST(M m) { lst=TRM; PF("|->"); prML(m->n); PF("\n"); }

I dbg=0, ch;
typedef struct ll { D d; struct ll *n; } *LL; I sLL=sizeof(struct ll); 
M idx(M oM,I t) { I i=0; M m=oM; while (m->n&&i++<t) m=m->n; R m; }
V mch(M m,D r,LL ms) { if (!m) R; D cR=r; while (cR) { //PF("(%s %s) ",m->w,cR->w);
	if (cR->p) { if (!(cR->p(m))) R; } else if (strcmp(m->w,cR->w)) R; LL lst; MAP(ms,); lst=c;
	if (!(cR->c)) { LL n=ma(sLL); n->d=cR; n->n=0; lst->n=n; R; } {MAP(cR->c,mch(m->n,c,ms))} cR=cR->n; } }
V ex(M m,D r) { I i=0, l; LL ms=ma(sLL); ms->n=0; while (1) { l=0; {MAP(m->n,l++)} if (i>=l) break;
	M iM=idx(m,i); ms->n=0; MAP(ms->n,fr(c)); mch(iM->n,r,ms); i++;
	if (ms->n) { ch=1; LL r=ms->n; MAP(ms->n,r=c->d->l>r->d->l?c:r); r->d->f(iM); i=0; if (dbg) prAST(m); } } }
I main(I ac,C *av) { D a=nID(wP,0,"?a"), r=nD("r",rPtv), d=nD("d",dPtv), q=nD("q",qPtv),
		a2=nID(wP,0,"?a"), s=nD("s",sPtv), 
		q1=nID(qP,0,"?q"), u=nD("u",uPtv), q2=nID(qP,0,"q"), c=nD("c",cPtv);
	nC(a,r); nC(a,d); nC(a,q); nC(a,a2); nC(a2,s); a->n=q1; nC(q1,u); nC(q1,q2); nC(q2,c);
	for (I i=0;i<ac;i++) { if (!strcmp("-d",av[i])) dbg=1; }
	C t=ma(100); I i, l;
	while (gets(t)) { l=ch=i=0; M ast=nM(ST,""); P(t,&i,&ast->n); 
		if (i!=1+strlen(t)) { printf("X-> parsing error: mismatched parentheses\n"); goto end; }
		ex(ast,a); MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { fML(ast); exit(0); } 
		if (!dbg||!ch) prAST(ast); end: fML(ast); }
	fD(a); }
