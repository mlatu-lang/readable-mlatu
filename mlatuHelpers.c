typedef int I; typedef char *S; typedef void V;
#define C case
#define B break
#define R return
#define _ static
#define PF printf
#define FR(x) free(x)
#define MA(x) malloc(x)
#define SQ(s,t) (!strcmp(s,t))
#define P(c,...) if (c) R __VA_ARGS__ // panic
#define SC(d,s) (d=MA(strlen(s)+1),strcpy(d,s))
#define WS(c) ((c)==' '||(c)=='\t'||(c)=='\r'||(c)=='\n'||(c)<=0) // whitespace
#define DO(n,x) do { I _n=n; for (I i=0;i<_n;i++) { x; } } while (0) // no stinkin loops
#define MAP(_d,x) typeof(_d) c=_d, _n; for (;c;c=_n) { _n=c->n; x; if (!_n) B; } // map over linked list

_ D fnd(D d,S s) { MAP(d,P(SQ(c->w,s),c)); R 0; } // find

#define ER(er,ln) (E){SC(n,f),er,ln} // S n (copy to), f(ilename)
