#include <math.h>
#include "c.h"
#include "ps.c"
V rm(M m) { M n=m->n->n; fM(m->n); m->n=n; }
I wF(M M) { R 1; } I qF(M m) { R m->t==Q; } // ?w and ?q
V rPtv(M m) { rm(m); rm(m); } 
V dPtv(M m) { rm(m->n); M c=cM(m->n); c->n=m->n->n; m->n->n=c; }
V sPtv(M m) { M c=m->n; m->n=c->n; c->n=m->n->n; m->n->n=c; rm(m->n->n); }
//V qPtv(M m) { M q=m->n->n; q->w=""; q->t=Q; q->c=m->n; q->c->n=0; m->n=q; }
V qPtv(M m) { rm(m->n); M q=nM(Q,""); q->c=m->n; q->n=m->n->n; q->c->n=0; m->n=q; }
V uPtv(M m) { M cs=m->n->c; m->n->c=0; rm(m); rm(m); if (!cs) R; MAP(cs,); c->n=m->n; m->n=cs;  }
V cPtv(M m) { M q=m->n->n; MAP(m->n->c,); c?(c->n=q->c):(m->n->c=q->c); q->c=0; rm(m->n); rm(m->n); }
 
I lst; V prML(M m); V prM(M m) { lst==Q?lst=TRM:PF(" "); switch (m->t) { // print ast node
	case TRM: DO(i,strlen(m->w),I e=esc(m->w[i]); PF("%*s%c",e,e?"`":"",m->w[i])); break;
	case Q: PF("("); lst=Q; prML(m->c); PF(")"); lst=TRM; break; } }
V prML(M m) { MAP(m,prM(c)); } // prints full list of M 
V prAST(M m) { lst=TRM; PF("|->"); prML(m->n); PF("\n"); } // print ast
// todo: rewrite to ast, linked list of numbers of defs
I dbg=0, ch;
M idx(M oM,I t) { I i=0; M m=oM; while (m->n&&i++<t) m=m->n; R m; } // index M linked list
D mch(I i,M m,const D rs[]) /* find first match */ { DO(l,i,D r=rs[l];
	MAP(r,I fd=1;DO(n,l+1,M iM=idx(m,i-n);P p=c->p[n];fd*=p->f?p->f(iM):!strcmp(iM->w,p->w))if (fd) R c;)) R 0; }
V ex(M m,D rs[]) /* rewrites */ { I i=1; while (1) { I l=0; {MAP(m->n,l++)} if (i>l) break;
	D r=mch(i,m,rs); i++; if (r) { r->f(idx(m,i-r->l-1)); if (dbg) prAST(m); ch=i=1; } } }
I main(I ac,C *av) { 
	DO(i,ac-1,if (!strcmp("-d",av[i+1])) dbg=1; else { FILE *f=fopen(av[i+1],"r"); 
		if (!f) { PF("error opening file '%s'\n",av[i+1]); exit(-1); } pF(f); }); // flags and rules
	D rs[10]={0}; P wP=nP(wF,""); P qP=nP(qF,"");
	D r=nD(2,wP,nP(0,"r")); r->f=rPtv; rs[1]=r;    D d=nD(2,wP,nP(0,"d")); d->f=dPtv; r->n=d;
	D q=nD(2,wP,nP(0,"q")); q->f=qPtv; d->n=q;     D u=nD(2,qP,nP(0,"u")); u->f=uPtv; q->n=u;
	D s=nD(3,wP,wP,nP(0,"s")); s->f=sPtv; rs[2]=s; D c=nD(3,qP,qP,nP(0,"c")); c->f=cPtv; s->n=c;
	C t=ma(100); I i, l;
	while (fgets(t,100,stdin)) { t[strlen(t)-1]='\0'; l=ch=i=0; M ast=nM(ST,""); PS(t,&i,&ast->n); // repl
		if (i!=1+strlen(t)) { PF("X-> parsing error: mismatched parentheses\n"); goto end; }
		ex(ast,rs); MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { fML(ast); exit(0); } 
		if (!dbg||!ch) prAST(ast); end: fML(ast); }
	DO(i,10,fD(rs[i])); }
