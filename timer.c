#include "mlatuMacros.h"
#include "mlatu.h"
I main(){D r=newRoot();parseFile("a.mlt",r);I i;T ast=parseTerms("100 100 mul",&i);rewrite(r,ast);prettyTerms(ast);}
