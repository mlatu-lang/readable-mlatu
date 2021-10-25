#include "c.h"
M wd(C t,I st,I l,M *p) { C w=ma(l+1); strncpy(w,t+st,l); w[l]='\0'; M n=nM(TRM,w); fr(w); *p=n; R n; }
V P(C t,I *i,M *s) { I st=*i; M *c=s; /* n/c */ do switch (t[*i]) { 
	#define WD if (*i>st) c=&(wd(t,st,*i-st,c)->n)
	case ' ': case '\0': WD; st=*i+1; break;
	case '(': WD; M n=nM(Q,""); *c=n; c=&(n->n); (*i)++; P(t,i,&(n->c)); st=*i+1; break;
	case ')': WD; R; } while ((*i)++<strlen(t)); }
