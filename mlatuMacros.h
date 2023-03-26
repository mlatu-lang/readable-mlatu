#ifndef mlatuMacros_h
#define mlatuMacros_h

typedef int I; typedef char *S; typedef void V;
#define C case
#define B break
#define R return
#define _ static
#define PF printf
#define fr(x) free(x)
#define ma(x) malloc(x)
#define sc(d,s) strcpy(d,s)
#define P(x,...) if (x) R __VA_ARGS__ // panic
#define DO(n,x) { I _n=(n); for (I i=0;i<_n;i++) { x; } } // no stinkin loops
#define MAP(_d,x) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } } // map over linked list

#endif
