#ifndef c_h
#define c_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int I; typedef char *C; typedef void V; typedef enum { Q,TRM,ST } T; // ST starts each ast
// type, word, children, next
typedef struct m { I t; C w; struct m *c,*n; } *M;                          I sM=sizeof(struct m); // ast
// predicate, word, children, next, function, rewrite, length, empty rewrite?
typedef struct d { I (*p)(); C w; struct d *c,*n; V (*f)(); M r; I l, e; } *D; I sD=sizeof(struct d); // def
/*  rule matching layout
	mlatu: foo = x; foo bar = y; foo baz = z;
	internal representation:
    foo = x
       |
      ->c 
    bar = y  ->n  baz = z */

#define R return
#define B break
#define PF printf
#define ma(x) malloc(x)
#define ca(x) calloc(1,x)
#define fr(x) free(x)
#define MAP(_d,x) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } }
#define DO(n,x) { I _n=(n); for (I i=0;i<_n;i++) {x;} }
#define esc(x) ((x)==' '||(x)=='`'||(x)=='('||(x)==')'||(x)==';'||(x)=='=')

V fM(M m) { fr(m->w); MAP(m->c,fM(c)); fr(m); } V fML(M m) { MAP(m,fM(c)); } // free M, free list of M
M nM(I t,C w) { M m=ma(sM); m->t=t; m->w=ma(strlen(w)+1); strcpy(m->w,w); m->n=m->c=0; R m; } // new M
M cM(M m) { M z=nM(0,""), n=z; MAP(m,n=n->n=nM(c->t,c->w);n->c=cM(c->c)); n=z->n; fM(z); R n; } // clone M

V fD(D d) { MAP(d,fr(c->w);fD(c->c);fML(c->r);fr(c)) } // free linked list of D
D nDW(C w) { D d=ca(sD); d->w=ma(strlen(w)+1); strcpy(d->w,w); R d; } // new rule with match on literal
// new internal rule (rewrite is fn, predicate is fn)
D nID(I (*p)(),V (*f)(),C w) { D d=nDW(w); d->f=f; d->p=p; R d; } 
D nRD(C w,M r) { D d=nDW(w); d->r=r; R d; } // new rewrite rule (rewrite is terms, predicate is literal)
D nD(C w,V (*f)()) { D d=nDW(w); d->f=f; R d; } // new rule (rewrite is fn, predicate is literal)
V nC(D p,D cd) { cd->l=p->l+1; if (p->c) { MAP(p->c,); c->n=cd; } else p->c=cd; } // adds child to rule
#endif
