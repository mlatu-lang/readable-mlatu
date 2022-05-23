#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlatuMacros.h"

Z V fT(T t) { fr(t->w); MAP(t->c,fT(c)); fr(t); } V freeTerms(T t) { MAP(t,fT(c)); } // free T
Z T nT(I t,C w) { T z=ma(sizeof(struct t)); z->t=t; z->w=ma(strlen(w)+1); strcpy(z->w,w); z->n=z->c=0; R z; } // new T
Z T cT(T t) { T z=nT(0,""), n=z; MAP(t,n=n->n=nT(c->t,c->w);n->c=cT(c->c)); n=z->n; fT(z); R n; } // clone T

enum { Q,TRM,ST }; // ST starts each ast
V freeRules(D d) { MAP(d,fr(c->w);freeRules(c->c);freeTerms(c->r);fr(c)); }
Z D nD(C w,T r) { D d=calloc(1,sizeof(struct d)); d->w=ma(strlen(w)+1); strcpy(d->w,w); d->r=r; R d; } // new D
D newRoot() { D root=nD("",0); R root; }

Z T wd(C s,I st,I l,T *p) { C w=ma(l+1); strncpy(w,s+st,l); w[l]=0; T n=nT(TRM,w); fr(w); R *p=n; }
Z V P(C s,I *i,I *er,I lvl,T *c/* n/c */) { I st=*i; do switch (s[*i]) { // parser
	#define WD if (*i>st) c=&(wd(s,st,*i-st,c)->n); st=*i+1
	#define ER(x) if (x>*er) *er=x;
	case ' ': case '\0': case '\n': case '\t': case '\r': WD; B;
	case '=': WD; c=&(*c=nT(TRM,"="))->n; ER(EQ); B; case '.': WD; c=&(*c=nT(TRM,"."))->n; ER(PRD); B;
	case '+': case '-': case '<': case '>': case '~': case ',': WD; char t[2]={s[*i],0}; c=&(*c=nT(TRM,t))->n; B;
	case '(': WD; T n=nT(Q,""); *c=n; c=&(n->n); (*i)++; P(s,i,er,lvl+1,&(n->c)); st=*i+1; B;
	case ')': WD; if (lvl==0) ER(PRN); R; } while ((*i)++<strlen(s)); if (lvl) ER(PRN); }
T parseTerms(C s,I *er) { T t=nT(ST,""); I i=0; *er=0; P(s,&i,er,0,&t->n); R t; }
I parseRule(C s,D d) { I l=strlen(s), cm=0; C nS=ma(l+1); strcpy(nS,s); DO(l,if(cm=cm?s[i]!='\n':s[i]=='#')nS[i]=' ');
	I e; T t, oT; t=oT=parseTerms(nS,&e); fr(nS); if (e==PRN) goto end;
	I eq=0, per=0; MAP(t,if(*c->w=='=')eq++;if(*c->w=='.')per++;if(!eq&&c->c){e=MCH;goto end;});
	e=eq!=1?EQ:*t->n->w=='='?EMPTY:per!=1?PRD:*c->w!='.'?END:0; if (e) goto end;
	{MAP(t,if(*c->n->w=='.'){fT(c->n);c->n=0;B;})}
	t=t->n; D n; while (1) { if (*t->w=='=') { freeTerms(d->r); d->r=t->n; if (!t->n) d->e=1; t->n=0; B; }
		n=0; MAP(d->c,if(!strcmp(t->w,c->w))n=c);
		if (!n||!d->c) { n=nD(t->w,0); n->l=d->l+1; if (d->c) c->n=n; else d->c=n; } d=n; t=t->n; }
	end: freeTerms(oT); R e; } 
I parseFile(C n,D root) { FILE *f=fopen(n,"rb"); if (!f) R OPEN; I pos, l=0, d, st=ftell(f), r=0, cm=0;
	while (1) { d=fgetc(f); if (d==-1) B; if (d!=' '&&d!='\n'&&d!='\t'&&d!='\r'&&d!='#'&&!cm) r=1; l++;
		if (!(cm=cm?d!='\n':d=='#')&&d=='.') { pos=ftell(f); C s=ma(l+1); fseek(f,st,SEEK_SET); fread(s,1,l,f); s[l]=0;
			I er=parseRule(s,root); fr(s); if (er) R fclose(f), er; l=0; fseek(f,pos,SEEK_SET); st=ftell(f); r=0; } }
	fclose(f); R r*END; }
I parseRules(C s,D root) { I l=0, d, r=0, cm=0;
	DO(strlen(s), d=s[i]; if (d!=' '&&d!='\n'&&d!='\t'&&d!='\r'&&d!='#'&&!cm) r=1; l++;
		if (!(cm=cm?d!='\n':d=='#')&&d=='.') { C nS=ma(l+1); strncpy(nS,s+i+1-l,l); nS[l]=0;
			I er=parseRule(nS,root); fr(nS); if (er) R er; l=r=0; });
	R r*END; }

Z I len(T t) { if (t->t==TRM) R strlen(t->w); else { I i=2; MAP(t->c,i+=len(c)+!!c->n) R i; } } // length of printed T
Z V prT(T t); C s; I i; V prTL(T t) { MAP(t,prT(c);if(c->n)s[i++]=' '); } // print T list
Z V prT(T t) { if (t->t==TRM) i+=strlen(strcpy(s+i,t->w)); else { s[i++]='('; prTL(t->c); strcpy(s+i++,")"); } } // print T
C prettyTerms(T t) { t=t->t==ST?t->n:t; I l=i=0; MAP(t,l+=len(c)+!!c->n); s=ma(l+1); prTL(t); R s; }
C prettyTerm(T t) { I l=len(t); i=0; s=ma(l+1); prT(t); R s; }

Z V rm(T t) { T n=t->n->n; fT(t->n); t->n=n; } // removes next T
Z V zap (T t) { rm(t); rm(t); } 
Z V swap(T t) { T c=t->n; t->n=c->n; c->n=t->n->n; t->n->n=c; rm(t->n->n); }
Z V copy(T t) { rm(t->n); T n=t->n->n; t->n->n=0; T c=cT(t->n); t->n->n=c; c->n=n; }
Z V wrap(T t) { T e=t->n, q=e->n; strcpy(q->w,""); q->t=Q; q->c=e; e->n=0; t->n=q; }
Z V exec(T t) { T cs=t->n->c; t->n->c=0; rm(t); rm(t); if (!cs) R; MAP(cs,); c->n=t->n; t->n=cs;  }
Z V cat (T t) { T q=t->n->n; MAP(t->n->c,); c?(c->n=q->c):(t->n->c=q->c); q->c=0; rm(t->n); rm(t->n); }

Z I lit(T t,D r) { D cR=r, bst=0; T u=t->n; // try to find rewrite on terms starting with literal
	l: {MAP(cR->c,if (!strcmp(u->w,c->w)) { if (c->r||c->e) bst=c; cR=c; if (u=u->n) goto l; else B; })}
	if (bst) { DO(bst->l,rm(t)); T z=cT(bst->r); if (z) { MAP(z,); c->n=t->n; t->n=z; }} R !!bst; }
Z I qot(T s) { T t=s->n, u=t->n; if (!u) R 0; // try to find rewrite on terms starting with quote
	if (u->t==Q) { if (!u->n) R 0; char w=*u->n->w; if (w=='~') swap(s); else if (w==',') cat(s); else R 0; R 1; }
	else switch (*u->w) { case '-': zap(s); R 1; case '+': copy(s); R 1; case '<': exec(s); R 1; case '>': wrap(s); R 1; } R 0; }
Z I ex(T oT,D r,I stp) /* rewrite alg */ { T t=oT; I i=0, n=0; while (1) { I l=0; {MAP(oT,l++)} if (i>l-2) R stp+n;
	if (t->n->t==Q?qot(t):lit(t,r)) { if (stp) R 0; else t=oT; i=0; n++; } else { t=t->n; i++; } } }
I rewrite(D r,T t) { R ex(t,r,0); } I stepRewrite(D r,T t) { R ex(t,r,1); }
