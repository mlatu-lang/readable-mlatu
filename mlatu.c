#include "mlatuMacros.h"
#include "mlatu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// free T, free list of T, new T, clone T
V fT(T t) { fr(t->w); MAP(t->c,fT(c)); fr(t); } V fTL(T t) { MAP(t,fT(c)); } V freeTerms(T t) { fTL(t); }
T nT(I t,C w) { T z=ma(sizeof(struct t)); z->t=t; z->w=ma(strlen(w)+1); strcpy(z->w,w); z->n=z->c=0; R z; }
T cT(T t) { T z=nT(0,""), n=z; MAP(t,n=n->n=nT(c->t,c->w);n->c=cT(c->c)); n=z->n; fT(z); R n; }

enum { Q,TRM,ST }; // ST starts each ast
V fD(D d) { MAP(d,fr(c->w);fD(c->c);fTL(c->r);fr(c)) } V freeRules(D r) { fD(r); } // free linked list of D
D nDW(C w) { D d=ca(sizeof(struct d)); d->w=ma(strlen(w)+1); strcpy(d->w,w); R d; } // new rule, match on literal
D nID(I (*p)(),V (*f)(),C w) { D d=nDW(w); d->f=f; d->p=p; R d; } // internal rule (match on quote, rewrite is fn)
D nRD(C w,T r) { D d=nDW(w); d->r=r; R d; } // new rewrite rule (rewrite is terms, predicate is literal)
D nD(C w,V (*f)()) { D d=nDW(w); d->f=f; R d; } // new rule (rewrite is fn, predicate is literal)
V nC(D p,D cd) { cd->l=p->l+1; if (p->c) { MAP(p->c,); c->n=cd; } else p->c=cd; } // adds child to rule

#define esc(x) ((x)==' '||(x)=='`'||(x)=='('||(x)==')'||(x)==';'||(x)=='=')
T wd(C t,I st,I l,I e,T *p) { C w=ma(l-e+1); I j=0; DO(l,w[j++]=t[st+(i=t[st+i]=='`'&&esc(t[st+i+1])?i+1:i)])
	w[l-e]='\0'; T n=nT(TRM,w); fr(w); R *p=n; }
V P(C t,I *i,I *er,I lvl,T *s) { I st=*i, e=0; T *c=s; /* n/c */ do switch (t[*i]) { // parser
	#define WD if (*i>st) c=&(wd(t,st,*i-st,e,c)->n), e=0; st=*i+1
	case '`': if (esc(t[*i+1])) e++,(*i)++; else *er=UNESC; B;
	case ' ': case '\0': case '\n': case '\t': WD; B;
	case '=': WD; c=&(*c=nT(TRM,"="))->n; *er=EQ; B; case ';': WD; c=&(*c=nT(TRM,";"))->n; *er=SEMI; B;
	case '+': case '-': case '<': case '>': case '~': case ',': WD; char s[2]={t[*i],0}; c=&(*c=nT(TRM,s))->n; B;
	case '(': WD; T n=nT(Q,""); *c=n; c=&(n->n); (*i)++; P(t,i,er,lvl+1,&(n->c)); st=*i+1; B;
	case ')': WD; if (lvl==0) *er=PRN; R; } while ((*i)++<strlen(t)); if (lvl) *er=PRN; }
T parseTerms(C s,I *er) { T t=nT(ST,""); I i=0; *er=0; P(s,&i,er,0,&t->n); R t; }
I parseRule(C s,D root) { I e=0; T t=parseTerms(s,&e); if (e==PRN||e==UNESC) R e;
	I eq=0, semi=0; MAP(t,if(!strcmp(c->w,"="))eq++;if(!strcmp(c->w,";"))semi++;if(!eq&&c->c)R MCH;);
	if (eq!=1) R EQ; if (!strcmp(t->n->w,"=")) R EMPTY; 
	if (semi!=1) R SEMI; if (strcmp(c->w,";")) R END; {MAP(t,if(!strcmp(c->n->w,";")){fT(c->n);c->n=0;B;})}
	t=t->n; D d=root, n; while (1) { if (!strcmp(t->w,"=")) { d->r=t->n; if (!t->n) d->e=1; t->n=0; B; }
		n=0; MAP(d->c,if(!strcmp(t->w,c->w))n=c); 
		if (!n||!d->c) { n=nRD(t->w,0); n->l=d->l+1; if (d->c) c->n=n; else d->c=n; } d=n; t=t->n; }
	fTL(t); R 0; } 
// parse file
I pF(C n,D root) { FILE *f=fopen(n,"r"); if (!f) R OPEN;
	I pos, l=0, d, st=ftell(f); while (1) { d=fgetc(f); if (d==-1) B; char c=d; l++;
		if (c==';') { pos=ftell(f); C s=ma(l+1); fseek(f,st,0); fread(s,1,l,f); s[l]='\0'; 
			I er=parseRule(s,root); if (er) R er; fr(s); l=0; fseek(f,pos,0); st=ftell(f); } }
	fclose(f); R 0; }

V rm(T t) { T n=t->n->n; fT(t->n); t->n=n; }
I qP(T t) { R t->t==Q; } // ?q
V zapPtv(T t) { rm(t); rm(t); } 
V swapPtv(T t) { T c=t->n; t->n=c->n; c->n=t->n->n; t->n->n=c; rm(t->n->n); }
V copyPtv(T t) { rm(t->n); T n=t->n->n; t->n->n=0; T c=cT(t->n); t->n->n=c; c->n=n; }
V wrapPtv(T t) { T q=t->n->n; strcpy(q->w,""); q->t=Q; q->c=t->n; q->c->n=0; t->n=q; }
V execPtv(T t) { T cs=t->n->c; t->n->c=0; rm(t); rm(t); if (!cs) R; MAP(cs,); c->n=t->n; t->n=cs;  }
V catPtv(T t) { T q=t->n->n; MAP(t->n->c,); c?(c->n=q->c):(t->n->c=q->c); q->c=0; rm(t->n); rm(t->n); }

D newRoot() { D root=nD("",0), q1=nID(qP,0,"?q"), q2=nID(qP,0,"?q"); root->c=q1; nC(q1,q2);
		D zap=nD("-",zapPtv), copy=nD("+",copyPtv);   nC(q1,zap); nC(q1, copy);
		D exec=nD("<",execPtv), wrap=nD(">",wrapPtv); nC(q1,exec); nC(q1,wrap);
		D swap=nD("~",swapPtv), cat=nD(",",catPtv);   nC(q2,swap); nC(q2,cat); R root; }
 
V prTL(T t) { MAP(t,prettyTerm(c);if(c->n)PF(" ")); } // print term list
V prettyTerm(T t) { switch (t->t) { // print term node
	case TRM: DO(strlen(t->w),I e=esc(t->w[i]); PF("%*s%c",e,e?"`":"",t->w[i])); B;
	case Q: PF("("); prTL(t->c); PF(")"); B; } } V prettyTerms(T t) { prTL(t->n); }

T idx(T oT,I i) { I j=0; T t=oT; while (t->n&&j++<i) t=t->n; R t; } // index T linked list
V mch(T t,D r,D *bst) /* finds first match in t */ { if (!t) R; D cR=r; while (cR) { //PF("(%s %s) ",t->w,cR->w);
	if (cR->p) { if (!(cR->p(t))) goto cont; } else if (strcmp(t->w,cR->w)) goto cont;
	if ((cR->f||cR->r||cR->e)&&(!*bst||cR->l>(*bst)->l)) *bst=cR;
	{MAP(cR->c,mch(t->n,c,bst))} cont: cR=cR->n; } }
I ex(T t,D r,I stp) /* rewrite alg */ { I i=0, l; D bst; while (1) { l=0; bst=0; {MAP(t->n,l++)} if (i>=l) R 1;
	T iT=idx(t,i); mch(iT->n,r,&bst); i++;
	if (bst) {
		if (bst->f) bst->f(iT); else { DO(bst->l,rm(iT)); T z=cT(bst->r); if (z) { MAP(z,); c->n=iT->n; iT->n=z; } }
		i=0; if (stp) R 0; } } }
V rewrite(D r,T t) { ex(t,r->c,0); } I stepRewrite(D r,T t) { R ex(t,r->c,1); }
