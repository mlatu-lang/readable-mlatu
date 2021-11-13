typedef enum { OK,PRN,UNESC,SEMI,EQ } Er;
M wd(C t,I st,I l,I e,M *p) { C w=ma(l-e+1); I j=0; DO(l,w[j++]=t[st+(i=t[st+i]=='`'&&esc(t[st+i+1])?i+1:i)])
	w[l-e]='\0'; M n=nM(TRM,w); fr(w); R *p=n; }
V P(C t,I *i,I *er,I lvl,M *s) { I st=*i, e=0; M *c=s; /* n/c */ do switch (t[*i]) { // parser
	#define WD if (*i>st) c=&(wd(t,st,*i-st,e,c)->n), e=0
	case '`': if (esc(t[*i+1])) e++,(*i)++; else *er=UNESC; B;
	case ' ': case '\0': WD; st=*i+1; B;
	case '=': *er=EQ; B; case ';': *er=SEMI; B;
	case '(': WD; M n=nM(Q,""); *c=n; c=&(n->n); (*i)++; P(t,i,er,lvl+1,&(n->c)); st=*i+1; B;
	case ')': WD; if (lvl==0) *er=PRN; R; } while ((*i)++<strlen(t)); if (lvl) *er=PRN; }
V pF(FILE *f) { PF("%d",f); fclose(f); // parse file
}
