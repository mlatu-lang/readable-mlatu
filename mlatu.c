#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuMacros.h"

_ V fT(T t) /* free T */ { FR(t->w); MAP(t->c,fT(c)); FR(t); } V freeTerms(T t) { MAP(t,fT(c)); }
_ T nT(I t,S w) /* new T */ { T z=MA(sizeof(struct t)); z->t=t; SC(z->w,w); z->n=z->c=0; R z; }
_ T cT(T t) /* clone T */ { T z=nT(0,""), n=z; MAP(t,n=n->n=nT(c->t,c->w); n->c=cT(c->c)); n=z->n; fT(z); R n; }

enum { Q,TRM,ST }; // ST starts each ast
V freeRules(D d) { MAP(d,FR(c->w); freeRules(c->c); freeTerms(c->r); FR(c)); }
_ D nD(S w,T r) /* new D */ { D d=calloc(1,sizeof(struct d)); SC(d->w,w); d->r=r; R d; } D newRoot() { R nD("",0); }

_ T wd(S s,I st,I l,T *c) { S w=MA(l+1); strncpy(w,s+st,l); w[l]=0; T n=nT(TRM,w); FR(w); R *c=n; }
_ V Pr(S s,I *i,I *er,I lvl,T *c/* ptr to n/c */) /* parser */ { I st=*i, d; do switch (d=s[*i]) {
	#define WD if (*i>st) c=&(wd(s,st,*i-st,c)->n); st=*i+1
	#define ER(x) if (x>*er) *er=x
	C ' ': C '\0': C '\n': C '\t': C '\r': WD; B;
	C '=': C '.': C '+': C '-': C '<': C '>': C '~': C ',': WD; char t[2]={d}; c=&(*c=nT(TRM,t))->n; ER(d=='='?EQ:d=='.'?PRD:0); B;
	C '(': WD; T n=nT(Q,""); c=&(*c=n)->n; (*i)++; Pr(s,i,er,lvl+1,&n->c); st=*i+1; B;
	C ')': WD; if (!lvl) ER(PRN); R; } while ((*i)++<strlen(s)); if (lvl) ER(PRN); }
T parseTerms(S s,I *er) { T t=nT(ST,""); I i=0; *er=0; Pr(s,&i,er,0,&t->n); R t; }
_ I pR(S s,T rs) /* parse rule */ { I cm=0; S nS; SC(nS,s); DO(strlen(s),if (cm=cm?s[i]!='\n':s[i]=='#') nS[i]=' ');
	I e; T t; t=parseTerms(nS,&e); FR(nS); if (e==PRN) goto end;
	I eq=0; MAP(t,if (*c->w=='=') eq++; if (!eq&&c->c) { e=MCH; goto end; }); if (e=eq!=1?EQ:*t->n->w=='='?EMPTY:0) goto end;
	{MAP(t,if (*c->n->w=='.') { fT(c->n); c->n=0; B; })} T n=rs; while (n->c) n=n->c; n->c=t->n; end: fT(t); R e; }
_ V aR(T t,D d) /* add rule */ { while (1) { if (*t->w=='=') { freeTerms(d->r); d->r=t->n; if (!t->n) d->e=1; t->n=0; B; }
		D n=0; MAP(d->c,if (SQ(t->w,c->w)) n=c);
		if (!n||!d->c) { n=nD(t->w,0); n->l=d->l+1; if (d->c) c->n=n; else d->c=n; } d=n; t=t->n; } }
#define WS(c) ((c)==' '||(c)=='\t'||(c)=='\r'||(c)=='\n'||(c)<=0)
#define ER(g,s) SC(n,s),(E){n,g}
#define PRS(nm,fn,prel,cur,slice,next,end) E nm##H(S s,D root,T rs) { S n; SC(n,fn); prel; I l=0, r=0, w=0, x=0, cm=0, c, e;   \
	do { c=cur; l++; if (!WS(c)&&c!='#'&&!cm) r=1;  cm=cm?c!='\n':c=='#'; /*todo why no workkkkkk*/                              \
		if (!cm&&c=='.') { S nS=MA(l+1); e=l; slice; nS[l]=0; I g=pR(nS,rs); FR(nS); P(g,end,ER(g,fn)); l=r=0; }                   \
		if (w==7) { x++; if (WS(c)) { S nS=MA(e=x); slice; nS[x-1]=0; E g=parseFileH(nS,root,rs); FR(nS); P(g.e,end,g); w=x=0; } } \
		else if ("#wield "[w]==c) w++; else w=0; next; } while (c>0); end; P(r,ER(PRD,fn)); R (E){}; }                             \
E nm(S s,D root) { T rs=nT(0,""); E g=nm##H(s,root,rs); T o=rs; if (!g.e) while (rs=rs->c) aR(rs,root); freeTerms(o); R g; }
// +(c<0): when nothing (not even whitespace) after a wield in a file, file pos will be right after, not 1 after like normal
PRS(parseFile,  s,  FILE*f=fopen(s,"rb");P(!f,ER(OPEN,s)), fgetc(f), fseek(f,-e+(c<0),SEEK_CUR);fread(nS,1,e,f),    , fclose(f));
PRS(parseRules, "", I i,                                   s[i],     strncpy(nS,s+i+1-e,e),                      i++, 0        );

_ I len(T t) /* length of printed T */ { P(t->t==TRM,strlen(t->w)); I i=2; MAP(t->c,i+=len(c)+!!c->n) R i; }
_ V prT(T t); S s; I y; V prTL(T t) /* print T list */ { MAP(t,prT(c);if (c->n) s[y++]=' '); }
_ V prT(T t) /* print T */ { if (t->t==TRM) y+=strlen(strcpy(s+y,t->w)); else { s[y++]='('; prTL(t->c); strcpy(s+y++,")"); } }
S prettyTerms(T t) { t=t->t==ST?t->n:t; I l=y=0; MAP(t,l+=len(c)+!!c->n); s=MA(l+1); prTL(t); s[l]=0; R s; }
S prettyTerm(T t) { I l=len(t); y=0; s=MA(l+1); prT(t); s[l]=0; R s; }

_ V rm(T t) /* remove next T */ { T n=t->n->n; fT(t->n); t->n=n; }
_ V zap (T t) { rm(t); rm(t); }
_ V swap(T t) { T c=t->n; t->n=c->n; c->n=t->n->n; t->n->n=c; rm(t->n->n); }
_ V copy(T t) { rm(t->n); T n=t->n->n; t->n->n=0; T c=cT(t->n); t->n->n=c; c->n=n; }
_ V wrap(T t) { T e=t->n, q=e->n; q->t=Q; q->c=e; e->n=0; t->n=q; }
_ V exec(T t) { T cs=t->n->c; t->n->c=0; rm(t); rm(t); P(!cs,); MAP(cs,); c->n=t->n; t->n=cs; }
_ V cat (T t) { T q=t->n->n; MAP(t->n->c,); c?(c->n=q->c):(t->n->c=q->c); q->c=0; rm(t->n); rm(t->n); }

_ I lit(T t,D r) /* find rewrite on literal */ { D cR=r, bst=0; T u=t->n;
	l: {MAP(cR->c,if (SQ(u->w,c->w)) { if (c->r||c->e) bst=c; cR=c; if (u=u->n) goto l; B; })}
	if (bst) { DO(bst->l,rm(t)); T z=cT(bst->r); if (z) { MAP(z,); c->n=t->n; t->n=z; }} R !!bst; }
_ I qot(T s) /* find rewrite on quote */ { T t=s->n, u=t->n; P(!u,0);
	if (u->t==Q) { P(!u->n,0); char w=*u->n->w; if (w=='~') swap(s); else if (w==',') cat(s); else R 0; R 1; }
	switch (*u->w) { C '-': zap(s); R 1; C '+': copy(s); R 1; C '<': exec(s); R 1; C '>': wrap(s); R 1; } R 0; }
_ I ex(T oT,D r,I stp /* rewrite alg */ ) { T t=oT; I i=0, n=0; while (1) { P(!t->n,stp+n);
	if (t->n->t==Q?qot(t):lit(t,r)) { P(stp,0); t=oT; i=0; n++; } else { t=t->n; i++; } } }
I rewrite(D r,T t) { R ex(t,r,0); } I stepRewrite(D r,T t) { R ex(t,r,1); }
