#ifndef mlatuMacros_h 
#define mlatuMacros_h
typedef int I; typedef char *C; typedef void V; 
#define B break
#define R return
#define Z static
#define PF printf
#define fr(x) free(x)
#define ma(x) malloc(x)
#define DO(n,x) { I _n=(n); for (I i=0;i<_n;i++) { x; } }
#define MAP(_d,x) typeof(_d) c=_d, _n; if (c) { while (1) { _n=c->n; x; if (!_n) break; c=_n; } }
#endif
