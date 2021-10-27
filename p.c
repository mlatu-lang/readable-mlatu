M wd(C t,I st,I l,I e,M *p) { C w=ma(l-e+1); for (I i=st,n=0;n<l;i++) w[n++]=t[i=t[i]=='`'&&esc(t[i+1])?i+1:i];
	w[l-e]='\0'; M n=nM(TRM,w); fr(w); *p=n; R n; }
// todo: improve wd
V P(C t,I *i,M *s) { I st=*i, e=0; M *c=s; /* n/c */ do switch (t[*i]) { 
	#define WD if (*i>st) c=&(wd(t,st,*i-st,e,c)->n), e=0
	case '`': if (esc(t[*i+1])) e++,(*i)++; break;
	case ' ': case '\0': WD; st=*i+1; break;
	case '(': WD; M n=nM(Q,""); *c=n; c=&(n->n); (*i)++; P(t,i,&(n->c)); st=*i+1; break;
	case ')': WD; R; } while ((*i)++<strlen(t)); }
