#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuHelpers.c"

_ V fT(T t) /* free T */ { FR(t->w); MAP(t->c,fT(c)); FR(t); } V freeTerms(T t) { MAP(t,fT(c)); }
_ T nT(I t,S w) { T z=MA(sizeof(struct t)); z->t=t; SC(z->w,w); z->n=z->c=0; R z; } T newTerm(I t,S w) { R nT(t,w); }
_ T nTL(I t,S w,I ln) /* new T w/ line # */ { T z=nT(t,w); z->ln=ln; R z; }
_ T cT(T t) /* clone T */ { T z=nT(0,""), n=z; MAP(t,n=n->n=newTerm(c->t,c->w); n->c=cT(c->c)); n=z->n; fT(z); R n; }

V freeRules(D d) { MAP(d,FR(c->w); freeRules(c->c); freeTerms(c->r); FR(c)); }
_ D nD(S w) /* new D */ { D d=calloc(1,sizeof(struct d)); SC(d->w,w); R d; } D newRoot() { R nD(""); }

_ T wd(S s,I st,I l,T *c,I ln) { S w=MA(l+1); strncpy(w,s+st,l); w[l]=0; T n=nTL(TRM,w,ln); FR(w); R *c=n; }
_ E Pr(S s,I *i,I lvl,I *ln,T *c/* ptr to n/c */,S f) /* parse */ { E e=(E){}; I st=*i, d; S n; do switch (d=s[*i]) {
	#define PE(b,c) if (b&&c>e.e) { if (e.f) FR(e.f); e=ER(c,*ln); }
	#define WD if (*i>st) c=&(wd(s,st,*i-st,c,*ln)->n); st=*i+1
	C '\n': C ' ': C '\0': C '\t': C '\r': WD; if (d=='\n') (*ln)++; B;
	C '=': C '.': C '+': C '-': C '<': C '>': C '~': C ',': WD; char t[2]={d}; c=&(*c=nTL(TRM,t,*ln))->n;
		PE(d=='=',EQ); PE(d=='.',PRD); B;
	C '(': WD; T q=nTL(Q,"",*ln); c=&(*c=q)->n; (*i)++; I nE=Pr(s,i,lvl+1,ln,&q->c,f).e; PE(1,nE); st=*i+1; B;
	C ')': WD; PE(!lvl,PRN); R e; } while ((*i)++<strlen(s)); PE(lvl,PRN); R e; }
E pTH(S s,T *t,I *ln,S f) { *t=nT(ST,""); I i=0; R Pr(s,&i,0,ln,&(*t)->n,f); } E parseTerms(S s,T *t) { I x; R pTH(s,t,&x,""); }
_ V aR(T t,D d) /* add rule */ { MAP(t, if (*c->w=='=') { freeTerms(d->r); d->e=!(d->r=c->n); c->n=0; R; }
	D n=fnd(d->c,c->w); if (!n) { n=nD(c->w); n->l=d->l+1; if (d->c) { MAP(d->c,); c->n=n; } else d->c=n; } d=n); }
_ E cR(T t,S f) /* check rule */ { S n; P(*t->n->w=='=',ER(EMPTY,t->n->ln));
	I eq=0; MAP(t,if (*c->w=='=') eq++; P(eq==2,ER(EQ,c->ln)); P(c->c&&!eq,ER(MCH,c->ln))); P(!eq,ER(EQ,c->ln)); R (E){}; }
#define PRS(nm,fn,prel,cur,slce,next,end) E nm##H(S s,D root,T rs,T y,T p) { S f=fn, n; prel;                                \
	 I l=0, r=0, w=0, x=0, cm=0, ln=1, lm=1, c, m; do { c=cur; l++; if (c=='\n') lm++; cm=cm?c!='\n':c=='#'; if (!WS(c)&&!cm) r=1; \
		if (!cm&&c=='.') { S nS=MA(l+1); m=l; slce; nS[l]=0; I cm=0; DO(strlen(nS),if (cm=cm?nS[i]!='\n':nS[i]=='#') nS[i]=' ');     \
			T t; E e=pTH(nS,&t,&ln,f); FR(nS); if (e.e!=PRN) { if (e.f) FR(e.f); e=cR(t,f); } P(e.e,freeTerms(t),end,e);            \
			{MAP(t,if (*c->n->w=='.') { fT(c->n); c->n=0; B; })} while (rs->c) rs=rs->c; rs->c=t->n; fT(t); l=r=0; }	                       \
		if (w==7) { x++; if (WS(c)) {                                                                                             \
			S nS=MA(m=x); slce; nS[x-1]=0; MAP(y,); c->n=nT(0,nS); E e=parseFileH(nS,root,rs,y,p); FR(nS); P(e.e,end,e); w=x=0; } } \
		else if ("#wield "[w]==c) w++; else w=0; next; } while (c>0);                                                             \
	end; P(r,ER(PRD,lm)); R (E){}; }                                                                                           \
E nm(S s,D root,V(*cb)(S),T p) { T rs=nT(0,""), y=newTerm(0,""); E e=nm##H(s,root,rs,y,p);                                   \
	T o=rs; if (!e.e) { while (o=o->c) aR(o,root); MAP(y->n,cb(c->w)); } freeTerms(rs); freeTerms(y); R e; }
FILE *fF(S n,T p) { FILE *a; P(a=fopen(n,"rb"),a); MAP(p,S f=MA(strlen(n)+strlen(c->w)+2); strcpy(f,c->w); strcat(f,"/"); strcat(f,n);
	P(a=fopen(f,"rb"),a); FR(f)); R 0; };
// +(c<0): when nothing (not even whitespace) after a wield in a file, file pos will be right after, not 1 after like normal
PRS(parseFile,  s,  FILE *a=fF(s,p);P(!a,ER(OPEN,0)), fgetc(a), fseek(a,-m+(c<0),SEEK_CUR);fread(nS,1,m,a),    , fclose(a));
PRS(parseRules, "", I i,                              s[i],     strncpy(nS,s+i+1-m,m),                      i++, 0        );

_ I len(T t) /* length of printed T */ { P(t->t==TRM,strlen(t->w)); I i=2; MAP(t->c,i+=len(c)+!!c->n) R i; }
I j; S s; _ V prT(T t); V prTL(T t) /* print T list */ { MAP(t,prT(c); if (c->n) s[j++]=' '); }
_ V prT(T t) /* print T */ { if (t->t==TRM) j+=strlen(strcpy(s+j,t->w)); else s[j++]='(', prTL(t->c), strcpy(s+j++,")"); }
S prettyTerms(T t) { t=t->t==ST?t->n:t; I l=j=0; MAP(t,l+=len(c)+!!c->n); s=MA(l+1); prTL(t); s[l]=0; R s; }

_ V rm(T t) /* remove next T */ { T n=t->n->n; fT(t->n); t->n=n; }
_ V zap (T t) { rm(t); rm(t); }
_ V swap(T t) { T c=t->n; t->n=c->n; c->n=t->n->n; t->n->n=c; rm(t->n->n); }
_ V copy(T t) { rm(t->n); T n=t->n->n; t->n->n=0; T c=cT(t->n); t->n->n=c; c->n=n; }
_ V wrap(T t) { T e=t->n, q=e->n; q->t=Q; q->c=e; e->n=0; t->n=q; }
_ V exec(T t) { T cs=t->n->c; t->n->c=0; rm(t); rm(t); P(!cs,); MAP(cs,); c->n=t->n; t->n=cs; }
_ V cat (T t) { T q=t->n->n; MAP(t->n->c,); c?(c->n=q->c):(t->n->c=q->c); q->c=0; rm(t->n); rm(t->n); }

_ I lit(T t,D r) /* rewrite on literal */ { D bst=0; MAP(t->n,if (c->t!=TRM||!(r=fnd(r->c,c->w))) B; if (r->r||r->e) bst=r);
	P(!bst,0); DO(bst->l,rm(t)); T z=cT(bst->r); if (z) { MAP(z,); c->n=t->n; t->n=z; } R 1; }
_ I qot(T s) /* rewrite on quote */ { T t=s->n, u=t->n; P(!u,0);
	if (u->t==Q) { P(!u->n,0); I w=*u->n->w; if (w=='~') swap(s); else if (w==',') cat(s); else R 0; R 1; }
	switch (*u->w) { C '-': zap(s); R 1; C '+': copy(s); R 1; C '<': exec(s); R 1; C '>': wrap(s); R 1; } R 0; }
I stepRewrite(D r,T t) { MAP(t,P(c->n&&(c->n->t==Q?qot(c):lit(c,r)),0)); R 1; }
I rewrite(D r,T t) { I n=0; while (!stepRewrite(r,t)) n++; R n; }
