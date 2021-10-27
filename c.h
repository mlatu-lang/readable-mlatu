#ifndef c_h
#define c_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int I; typedef char *C; typedef void V; typedef enum { Q,TRM,ST } T;
// type, word, children, next
typedef struct m { I t; C w; struct m *c,*n; } *M;                          I sM=sizeof(struct m); // ast
// predicate, word, children, next, function, rewrite, length
typedef struct d { I (*p)(); C w; struct d *c,*n; V (*f)(); M r; I l; } *D; I sD=sizeof(struct d); // def
/* rule matching layout
   mlatu: foo -> x; foo bar -> y; foo baz -> z;
   C: D foo, bar, baz; foo->w="foo" etc; foo->r=ast with x then NULL etc; foo->c=bar; bar->n=baz; */
#define R return
#define PF printf
#define ma(x) malloc(x)
#define ca(x) calloc(1,x)
#define fr(x) free(x)
#define MAP(_d,x) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } }
#define DO(n,x) { I _n=(n); for (I i=0;i<_n;i++) {x;} }
#define esc(x) ((x)==' '||(x)=='`'||(x)=='('||(x)==')')
M nM(I t,C w) { M m=ma(sM); m->t=t; m->w=ma(strlen(w)+1); strcpy(m->w,w); m->n=m->c=0; R m; } // new M
// clone M
M cM(M m) { M z=nM(m->t,m->w); if (m->c) { M oc=m->c, nc=cM(oc); MAP(oc->n,nc=nc->n=cM(c)); z->c=nc; } R z; }
V fM(M m) { fr(m->w); MAP(m->c,fM(c)); fr(m); } V fML(M m) { MAP(m,fM(c)); } // free M
V fD(D d) { MAP(d,fr(c->w);fD(c->c);fML(c->r);fr(c)) } // free D
// new internal rule (rewrite is fn, predicate is fn)
D nID(I (*p)(),V (*f)(),C w) { D d=ca(sD); d->w=w; d->f=f; d->p=p; R d; } 
D nDW(C w) { D d=ca(sD); d->w=ma(strlen(w)+1); strcpy(d->w,w); R d; } // new rule with match on literal
D nRD(C w,M r) { D d=nDW(w); d->r=r; R d; } // new rewrite rule (rewrite is terms, predicate is literal)
D nD(C w,V (*f)()) { D d=nDW(w); d->f=f; R d; } // new rule (rewrite is fn, predicate is literal)
V nC(D p,D cd) { cd->l=p->l+1; if (p->c) { MAP(p->c,); c->n=cd; } else p->c=cd; } // adds child to rule
#endif
