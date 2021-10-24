#ifndef c_h
#define c_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int I; typedef char *C; typedef void V; typedef enum { Q,TRM,ST } T;
typedef struct m { I t; C w; struct m *c,*n; } *M;                     I sM=sizeof(struct m); // ast
typedef struct d { I (*p)(); C w; struct d *c,*n; V (*f)(); I l; } *D; I sD=sizeof(struct d); // def
#define R return
#define PF printf
#define ma(x) malloc(x)
#define fr(x) free(x)
#define MAP(_d,x...) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } }
D nID(I (*p)(),V (*f)(),C w) { D d=ma(sD); d->w=w; d->f=f; d->p=p; d->l=d->c=d->n=0; R d; }
D nD(C w,V (*f)()) { D d=ma(sD); d->w=ma(strlen(w)+1); strcpy(d->w,w); d->f=f; d->l=d->p=d->c=d->n=0; R d; }
V nC(D p,D cd) { cd->l=p->l+1; if (p->c) { MAP(p->c,); c->n=cd; } else p->c=cd; }
V fD(D d) { MAP(d,fr(c->w);fD(c->c);fr(c)) } 
V fM(M m) { fr(m->w); MAP(m->c,fM(c)); fr(m); } V fML(M m) { MAP(m,fM(c)); }
M nM(I t,C w) { M m=ma(sM); m->t=t; m->w=ma(strlen(w)+1); strcpy(m->w,w); m->n=m->c=0; R m; }
M cM(M m) { M z=nM(m->t,m->w); if (m->c) { M oc=m->c, nc=cM(oc); MAP(oc->n,nc=nc->n=cM(c)); z->c=nc; } R z; }

M wd(C t,I st,I l,M *p) { C w=ma(l+1); strncpy(w,t+st,l); w[l]='\0'; M n=nM(TRM,w); fr(w); *p=n; R n; }
V P(C t,I *i,M *s) { I st=*i; M *c=s; /* n/c */ do switch (t[*i]) { 
	#define WD if (*i>st) c=&(wd(t,st,*i-st,c)->n)
	case ' ': case '\0': WD; st=*i+1; break;
	case '(': WD; M n=nM(Q,""); *c=n; c=&(n->n); (*i)++; P(t,i,&(n->c)); st=*i+1; break;
	case ')': WD; R; } while ((*i)++<strlen(t)); }
#endif
