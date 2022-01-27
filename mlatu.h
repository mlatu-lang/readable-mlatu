#ifndef mlatu_h
#define mlatu_h
typedef int I; typedef char *C; typedef void V; 
// type, word, children, next
typedef struct t { I t; C w; struct t *c,*n; } *T, *term;
// word, children, next, function, rewrite, length, match only on quote, rewrite to empty term?
typedef struct d { C w; struct d *c,*n; V (*f)(); T r; I l, q, e; } *D, *rule; // definition (R was taken)
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
otherwise, it will be nonzero -- one of
PRN: unbalanced parens, UNESC: unescaped backtick, SEMI: unescaped semicolon, EQ: unescaped equal sign
the term returned will always needs freeing regardless of whether there was an error or not
*/
enum { OK,PRN,UNESC,SEMI,EQ,EMPTY,END,MCH,OPEN };
term parseTerms(char *s,int *er);

/*
takes a string, and a root to add the rule to (does not free s)
same error scheme as above, but with different meanings (if there was an error, root will not be changed)
PRN: unbalanced parens, UNESC: unescaped backtick, SEMI: more or less than 1 semicolon in rule,
EQ: more or less than 1 equal sign in rule, EMPTY: match on nothing, END: semicolon is not at end of rule,
MCH: tried to match on quote
*/
int parseRule(char *s,rule root);

int pF(C n,D root);

void freeTerms(term m);
void prettyTerms(term t);
void prettyTerm(term t);
void prettyRules(rule r);
void rewrite(rule r,term t);
I stepRewrite(rule r,term t);
#endif
