#include "mlatuMacros.h"
#include "mlatu.h"
I main(){D r=newRoot();pF("a.mlt",r);I i;T ast=parseTerms("100 5 mul",&i);rewrite(r,ast);prettyTerms(ast);}
