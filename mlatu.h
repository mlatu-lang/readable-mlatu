#ifndef mlatu_h
#define mlatu_h
typedef int I; typedef char *C; typedef void V; 
// type, word, children, next
typedef struct t { I t; C w; struct t *c,*n; } *T, *term;
// predicate, word, children, next, function, rewrite, length, rewrite to empty term?
typedef struct d { I (*p)(); C w; struct d *c,*n; V (*f)(); T r; I l, e; } *D, *rule; // definition (R was taken)
/*  rule matching layout
	mlatu: foo = x; foo bar = y; foo baz = z;
	internal representation:
    foo = x
       |
      ->c 
    bar = y  ->n  baz = z */
rule newRoot(); void freeRules(rule r);

/*
takes a string, and a pointer to an error variable
if the parse was successful, er will be 0 (OK)
otherwise, it will be nonzero
PRN: unbalanced parens, UNESC: unescaped backtick, SEMI: unescaped semicolon,
EQ, QEQ: unescaped equal sign
the term returned always needs freed regardless of whether there was an error or not
*/
enum { OK,PRN,UNESC,SEMI,EQ,QEQ };
term parseTerms(char *s,int *er);

int pF(C n,D root);

void freeTerms(term m);
void prettyTerms(term t);
void prettyTerm(term t);
void rewrite(rule r,term t);
I stepRewrite(rule r,term t);
#endif
