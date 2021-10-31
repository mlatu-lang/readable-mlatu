#ifndef c_h
#define c_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
typedef int I; typedef char *C; typedef void V; typedef enum { Q,TRM,ST } T;
typedef struct m { I t; C w; struct m *c,*n; } *M;        I sM=sizeof(struct m); // ast: type, word, children, next
typedef struct p { C w; I (*f)(); } *P;                   I sP=sizeof(struct p); // pred: word, function
// def (rule): preds, rewrite, next, length
typedef struct d { P **p; V (*f)(); M r; struct d *n; I l; } *D; I sD=sizeof(struct d);
#define R return
#define PF printf
#define ma(x) malloc(x)
#define fr(x) free(x)
#define MAP(_d,x) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } }
#define DO(v,n,x) { I _n=(n); for (I v=0;v<_n;v++) {x;} }
#define esc(x) ((x)==' '||(x)=='`'||(x)=='('||(x)==')')
M nM(I t,C w) { M m=ma(sM); m->t=t; m->w=ma(strlen(w)+1); strcpy(m->w,w); m->n=m->c=0; R m; } // new M
// clone M
M cM(M m) { M z=nM(m->t,m->w); if (m->c) { M oc=m->c, nc=cM(oc); MAP(oc->n,nc=nc->n=cM(c)); z->c=nc; } R z; }
V fM(M m) { fr(m->w); MAP(m->c,fM(c)); fr(m); } V fML(M m) { MAP(m,fM(c)); } // free M, free M list
V fD(D d) { if (!d) R; fr(d->p); fML(d->r); fD(d->n); } // free D
P nP(I (*f)(),C w) { P p=ma(sP); p->f=f; p->w=w; R p; } // new predicate
D nD(I l,...) { va_list a; va_start(a,l); D d=calloc(1,sD); d->p=ma(sP*(l-1)); 
	DO(i,l,d->p[l-i-1]=va_arg(a,P)); va_end(a); d->l=l; R d; } // new rule
#endif
