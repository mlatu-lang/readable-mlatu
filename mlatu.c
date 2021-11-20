#include "c.h"
#include "p.c"
V rm(M m) { M n=m->n->n; fM(m->n); m->n=n; }
I wP(M M) { R 1; } I qP(M m) { R m->t==Q; } // ?w and ?q
V rPtv(M m) { rm(m); rm(m); } 
V sPtv(M m) { M c=m->n; m->n=c->n; c->n=m->n->n; m->n->n=c; rm(m->n->n); }
V dPtv(M m) { rm(m->n); M n=m->n->n; m->n->n=0; M c=cM(m->n); m->n->n=c; c->n=n; }
V qPtv(M m) { M q=m->n->n; strcpy(q->w,""); q->t=Q; q->c=m->n; q->c->n=0; m->n=q; }
V uPtv(M m) { M cs=m->n->c; m->n->c=0; rm(m); rm(m); if (!cs) R; MAP(cs,); c->n=m->n; m->n=cs;  }
V cPtv(M m) { M q=m->n->n; MAP(m->n->c,); c?(c->n=q->c):(m->n->c=q->c); q->c=0; rm(m->n); rm(m->n); }
 
I lst; V prML(M m); V prM(M m) { lst==Q?lst=TRM:PF(" "); switch (m->t) { // print ast node
	case TRM: DO(strlen(m->w),I e=esc(m->w[i]); PF("%*s%c",e,e?"`":"",m->w[i])); B;
	case Q: PF("("); lst=Q; prML(m->c); PF(")"); lst=TRM; B; } }
V prML(M m) { MAP(m,prM(c)); } // prints full list of M 
V prAST(M m) { lst=TRM; PF("|->"); prML(m->n); PF("\n"); } // print ast

I dbg=0, ch;
typedef struct ll { D d; struct ll *n; } *LL; I sLL=sizeof(struct ll);
M idx(M oM,I t) { I i=0; M m=oM; while (m->n&&i++<t) m=m->n; R m; } // index M linked list
V mch(M m,D r,LL ms) /* finds first match in m */ { if (!m) R; D cR=r; while (cR) { //PF("(%s %s) ",m->w,cR->w);
	if (cR->p) { if (!(cR->p(m))) goto cont; } else if (strcmp(m->w,cR->w)) goto cont; LL lst; MAP(ms,); lst=c;
	LL n=ma(sLL); n->d=cR; n->n=0; lst->n=n; if (!(cR->c)) R; {MAP(cR->c,mch(m->n,c,ms))} cont: cR=cR->n; } }
V ex(M m,D r) /* rewrites */ { I i=0, l; LL ms=ma(sLL); ms->n=0; while (1) { l=0; {MAP(m->n,l++)} if (i>=l) B;
	M iM=idx(m,i); ms->n=0; MAP(ms->n,fr(c)); mch(iM->n,r,ms); i++;
	if (ms->n) { ch=1; LL r=ms->n; MAP(ms->n,r=c->d->l>r->d->l?c:r); 
		if (r->d->f) r->d->f(iM); 
		else { DO(r->d->l,rm(iM)); M z=cM(r->d->r); if (z) { MAP(z,); c->n=iM->n; iM->n=z; } } 
		i=0; if (dbg) prAST(m); } } }
#define abrt { fD(q1); R -1; }
I file(C n,D root) { FILE *f=fopen(n,"r"); if (!f) { PF("error opening file '%s'\n",n); R 1; } R pF(f,n,root); }
I main(I ac,C *av) { D root=nD("",0), q1=nID(qP,0,"?q"), r=nD("r",rPtv), d=nD("d",dPtv), q=nD("q",qPtv),
		q2=nID(qP,0,"?q"), s=nD("s",sPtv), u=nD("u",uPtv), c=nD("c",cPtv);
	root->c=q1; nC(q1,r); nC(q1,d); nC(q1,q); nC(q1,q2); nC(q2,s); nC(q1,u); nC(q2,c); // set ptvs
	DO(ac-1,if (!strcmp("-d",av[i+1])) dbg=1; else if (file(av[i+1],root)) abrt;) // flags and files
	if (file("prelude.mlt",root)) abrt; C t=ma(100); I i, l, er;
	while (fgets(t,100,stdin)) { t[strlen(t)-1]='\0'; er=l=ch=i=0; M ast=nM(ST,""); P(t,&i,&er,0,&ast->n); // repl
		if (er) { switch (er) { 
			case PRN:          PF("X-> parsing error: unbalanced parentheses\n"); B;
			case UNESC:        PF("X-> parsing error: unescaped backtick\n"); B;
			case EQ: case QEQ: PF("X-> parsing error: unescaped equal sign. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
			case SEMI:         PF("X-> parsing error: unescaped semicolon. \
If you are trying to define a rule, they cannot be defined in the repl, you need to load it from a file.\n"); B;
		} goto end; }
		ex(ast,root->c); MAP(ast,l++); if (l==2&&!strcmp("bye",ast->n->w)) { fML(ast); B; } 
		if (!dbg||!ch) prAST(ast); end: fML(ast); } fD(q1); }
