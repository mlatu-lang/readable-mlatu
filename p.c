typedef enum { OK,PRN,UNESC,SEMI,EQ,QEQ } Er;
M wd(C t,I st,I l,I e,M *p) { C w=ma(l-e+1); I j=0; DO(l,w[j++]=t[st+(i=t[st+i]=='`'&&esc(t[st+i+1])?i+1:i)])
	w[l-e]='\0'; M n=nM(TRM,w); fr(w); R *p=n; }
V P(C t,I *i,I *er,I lvl,M *s) { I st=*i, e=0; M *c=s; /* n/c */ do switch (t[*i]) { // parser
	#define WD if (*i>st) c=&(wd(t,st,*i-st,e,c)->n), e=0; st=*i+1
	case '`': if (esc(t[*i+1])) e++,(*i)++; else *er=UNESC; B;
	case ' ': case '\0': case '\n': case '\t': WD; B;
	case '=': WD; c=&(*c=nM(TRM,"="))->n; *er=lvl?QEQ:EQ; B; case ';': *er=SEMI; B;
	case '+': case '-': case '<': case '>': case '~': case ',': WD; char s[2]={t[*i],0}; c=&(*c=nM(TRM,s))->n; B;
	case '(': WD; M n=nM(Q,""); *c=n; c=&(n->n); (*i)++; P(t,i,er,lvl+1,&(n->c)); st=*i+1; B;
	case ')': WD; if (lvl==0) *er=PRN; R; } while ((*i)++<strlen(t)); if (lvl) *er=PRN; }
#define ER(e) { PF("error parsing file '%s': "#e"\n",n); R 1; }
// parse file
I pF(FILE *f,C n,D root) { I pos, l=0, d, lst=1, st=ftell(f); while (1) { d=fgetc(f); if (d==-1) B; char c=d;
		if (c==';') { lst=1; pos=ftell(f); C s=ma(l+1); fseek(f,st,0); fread(s,1,l,f); s[l]='\0';
			M m=nM(ST,""); I i=0, er=0; P(s,&i,&er,0,&m->n); fr(s);
			if (er!=EQ) { if (er==PRN) ER(unbalanced parentheses); if (er==QEQ) ER(unescaped quoted equal sign);
				ER(an unquoted unescaped equal sign is needed in each rule); }
			I eq=0; MAP(m,if(!strcmp(c->w,"="))eq+=1); if (eq!=1) ER(each rule must have exactly one equal sign);
			if (!strcmp(m->n->w,"=")) ER(each rule must have a pattern it matches);
			m=m->n; D d=root, n; while (1) { if (!strcmp(m->w,"=")) { d->r=m->n; if (!m->n) d->e=1; m->n=0; B; }
				if (m->c) ER(quotes are opaque and cannot be matched on); n=0; MAP(d->c,if(!strcmp(m->w,c->w))n=c); 
				if (!n||!d->c) { n=nRD(m->w,0); n->l=d->l+1; if (d->c) c->n=n; else d->c=n; } d=n; m=m->n; }
			fML(m); l=0; fseek(f,pos,0); st=ftell(f); } 
		else lst=(c==' '||c=='\t'||c=='\n')&&lst?1:0, l++; }
	if (!lst) ER(semicolon expected at end of last rule); fclose(f); R 0; }
