#ifndef mlatuMacros_h
#define mlatuMacros_h

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
#define SC(d,s) d=MA(strlen(s)+1),strcpy(d,s)
#define DO(n,x) { I _n=n; for (I i=0;i<_n;i++) { x; } } // no stinkin loops
#define MAP(_d,x) typeof(_d) c=_d, _n; while (c) { _n=c->n; x; if (!_n) B; c=_n; } // map over linked list

#endif
