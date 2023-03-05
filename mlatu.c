#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuMacros.h"

_ V fT(T t) { fr(t->w); MAP(t->c,fT(c)); fr(t); } V freeTerms(T t) { MAP(t,fT(c)); } // free T
_ T nT(I t,S w) { T z=ma(sizeof(struct t)); z->t=t; z->w=ma(strlen(w)+1); sc(z->w,w); z->n=z->c=0; R z; } // new T
_ T cT(T t) { T z=nT(0,""), n=z; MAP(t,n=n->n=nT(c->t,c->w); n->c=cT(c->c)); n=z->n; fT(z); R n; } // clone T

enum { Q,TRM,ST }; // ST starts each ast
V freeRules(D d) { MAP(d,fr(c->w); freeRules(c->c); freeTerms(c->r); fr(c)); }
_ D nD(S w,T r) { D d=calloc(1,sizeof(struct d)); d->w=ma(strlen(w)+1); sc(d->w,w); d->r=r; R d; } // new D
D newRoot() { R nD("",0); }

_ T wd(S s,I st,I l,T *p) { S w=ma(l+1); strncpy(w,s+st,l); w[l]=0; T n=nT(TRM,w); fr(w); R *p=n; }
_ V Pr(S s,I *i,I *er,I lvl,T *c/* n/c */) { I st=*i; do switch (s[*i]) { // parse
	#define WD if (*i>st) c=&(wd(s,st,*i-st,c)->n); st=*i+1
	#define ER(x) if (x>*er) *er=x;
	C ' ': C '\0': C '\n': C '\t': C '\r': WD; B;
	C '=': WD; c=&(*c=nT(TRM,"="))->n; ER(EQ); B; C '.': WD; c=&(*c=nT(TRM,"."))->n; ER(PRD); B;
	C '+': C '-': C '<': C '>': C '~': C ',': WD; char t[2]={s[*i],0}; c=&(*c=nT(TRM,t))->n; B;
	C '(': WD; T n=nT(Q,""); *c=n; c=&(n->n); (*i)++; Pr(s,i,er,lvl+1,&(n->c)); st=*i+1; B;
	C ')': WD; if (lvl==0) ER(PRN); R; } while ((*i)++<strlen(s)); if (lvl) ER(PRN); }
T parseTerms(S s,I *er) { T t=nT(ST,""); I i=0; *er=0; Pr(s,&i,er,0,&t->n); R t; }
I parseRule(S s,T rs) { I l=strlen(s), cm=0; S nS=ma(l+1); sc(nS,s); DO(l,if (cm=cm?s[i]!='\n':s[i]=='#') nS[i]=' ');
	I e; T t; t=parseTerms(nS,&e); fr(nS); if (e==PRN) goto end;
	I eq=0, per=0; MAP(t,if (*c->w=='=') eq++; if (*c->w=='.') per++; if (!eq&&c->c) { e=MCH; goto end; });
	if (e=eq!=1?EQ:*t->n->w=='='?EMPTY:per!=1?PRD:*c->w!='.'?END:0) goto end;
	{MAP(t,if (*c->n->w=='.') { fT(c->n); c->n=0; B; })} T n=rs; while (n->c) n=n->c; n->c=t->n; fT(t); end: R e; }
V aR(T t,D d) { D n; while (1) { if (*t->w=='=') { freeTerms(d->r); d->r=t->n; if (!t->n) d->e=1; t->n=0; B; } // add rule
		n=0; MAP(d->c,if (!strcmp(t->w,c->w)) n=c);
		if (!n||!d->c) { n=nD(t->w,0); n->l=d->l+1; if (d->c) c->n=n; else d->c=n; } d=n; t=t->n; } }
I parseFile(S n,D root) { FILE *f=fopen(n,"rb"); P(!f,OPEN); I pos, l=0, d, st=ftell(f), r=0, cm=0; T rs=nT(0,"");
	while (1) { d=fgetc(f); if (d==-1) B; if (d!=' '&&d!='\n'&&d!='\t'&&d!='\r'&&d!='#'&&!cm) r=1; l++;
		if (!(cm=cm?d!='\n':d=='#')&&d=='.') { pos=ftell(f); S s=ma(l+1); fseek(f,st,SEEK_SET); fread(s,1,l,f); s[l]=0;
			I er=parseRule(s,rs); fr(s); P(er,freeTerms(rs),fclose(f),er); l=0; fseek(f,pos,SEEK_SET); st=ftell(f); r=0; } }
	T o=rs; if (!r) while (rs=rs->c) aR(rs,root); freeTerms(o); fclose(f); R r*END; }
I parseRules(S s,D root) { I l=0, d, r=0, cm=0; T rs=nT(0,"");
	DO(strlen(s), d=s[i]; if (d!=' '&&d!='\n'&&d!='\t'&&d!='\r'&&d!='#'&&!cm) r=1; l++;
		if (!(cm=cm?d!='\n':d=='#')&&d=='.') { S nS=ma(l+1); strncpy(nS,s+i+1-l,l); nS[l]=0;
			I er=parseRule(nS,rs); fr(nS); P(er,freeTerms(rs),er); l=r=0; });
	T o=rs; if (!r) while (rs=rs->c) aR(rs,root); freeTerms(o); R r*END; }

_ I len(T t) { P(t->t==TRM,strlen(t->w)); I i=2; MAP(t->c,i+=len(c)+!!c->n) R i; } // length of printed T
_ V prT(T t); S s; I i; V prTL(T t) { MAP(t,prT(c);if (c->n) s[i++]=' '); } // print T list
_ V prT(T t) { if (t->t==TRM) i+=strlen(sc(s+i,t->w)); else { s[i++]='('; prTL(t->c); strcpy(s+i++,")"); } } // print T
S prettyTerms(T t) { t=t->t==ST?t->n:t; I l=i=0; MAP(t,l+=len(c)+!!c->n); s=ma(l+1); prTL(t); s[l]=0; R s; }
S prettyTerm(T t) { I l=len(t); i=0; s=ma(l+1); prT(t); s[l]=0; R s; }

_ V rm(T t) { T n=t->n->n; fT(t->n); t->n=n; } // removes next T
_ V zap (T t) { rm(t); rm(t); }
_ V swap(T t) { T c=t->n; t->n=c->n; c->n=t->n->n; t->n->n=c; rm(t->n->n); }
_ V copy(T t) { rm(t->n); T n=t->n->n; t->n->n=0; T c=cT(t->n); t->n->n=c; c->n=n; }
_ V wrap(T t) { T e=t->n, q=e->n; sc(q->w,""); q->t=Q; q->c=e; e->n=0; t->n=q; }
_ V exec(T t) { T cs=t->n->c; t->n->c=0; rm(t); rm(t); P(!cs,); MAP(cs,); c->n=t->n; t->n=cs; }
_ V cat (T t) { T q=t->n->n; MAP(t->n->c,); c?(c->n=q->c):(t->n->c=q->c); q->c=0; rm(t->n); rm(t->n); }

_ I lit(T t,D r) { D cR=r, bst=0; T u=t->n; // try to find rewrite on terms starting with literal
	l: {MAP(cR->c,if (!strcmp(u->w,c->w)) { if (c->r||c->e) bst=c; cR=c; if (u=u->n) goto l; B; })}
	if (bst) { DO(bst->l,rm(t)); T z=cT(bst->r); if (z) { MAP(z,); c->n=t->n; t->n=z; }} R !!bst; }
_ I qot(T s) { T t=s->n, u=t->n; P(!u,0); // try to find rewrite on terms starting with quote
	if (u->t==Q) { P(!u->n,0); char w=*u->n->w; if (w=='~') swap(s); else if (w==',') cat(s); else R 0; R 1; }
	switch (*u->w) { C '-': zap(s); R 1; C '+': copy(s); R 1; C '<': exec(s); R 1; C '>': wrap(s); R 1; } R 0; }
_ I ex(T oT,D r,I stp) /* rewrite alg */ { T t=oT; I i=0, n=0; while (1) { P(!t->n,stp+n);
	if (t->n->t==Q?qot(t):lit(t,r)) { P(stp,0); t=oT; i=0; n++; } else { t=t->n; i++; } } }
I rewrite(D r,T t) { R ex(t,r,0); } I stepRewrite(D r,T t) { R ex(t,r,1); }
