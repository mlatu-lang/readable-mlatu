#ifndef mlatuMacros_h 
#define mlatuMacros_h
typedef int I; typedef char *C; typedef void V; 
#define R return
#define B break
#define PF printf
#define ma(x) malloc(x)
#define ca(x) calloc(1,x)
#define fr(x) free(x)
#define MAP(_d,x) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } }
#define DO(n,x) { I _n=(n); for (I i=0;i<_n;i++) { x; } }
#endif
