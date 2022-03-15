#include <stdio.h>
#include "../mlatu.h"
#include "../mlatuMacros.h"
I main(){D r=newRoot();parseFile("examples/nats.mlt",r);I i;T ast=parseTerms("100 100 mul",&i);rewrite(r,ast);prettyTerms(ast);PF("\n");}
