#ifndef mlatu_h
#define mlatu_h

// type, word, children, next
typedef struct t { int t; char *w; struct t *c,*n; } *T, *term;
void freeTerms(term m);
char *prettyTerms(term t); // pretty prints a list of terms, returning the allocated string
char *prettyTerm(term t); // pretty prints a single term, returning the allocated string

// word, children, next, function, rewrite, total length length (of rule + parents), match only on quote, rewrite to empty term?
typedef struct d { char *w; struct d *c,*n; void (*f)(); term r; int l, q, e; } *D, *rule; // definition (R was taken)
/*  rule matching layout
	mlatu: foo = x; foo bar = y; foo baz = z;
	internal representation:
    foo = x
       |
      ->c 
    bar = y  ->n  baz = z */
rule newRoot();
void freeRules(rule r);

/*
takes a string, and a pointer to an error variable
if the parse was successful, er will be 0 (OK)
otherwise, it will be nonzero -- one of
PRN: unbalanced parens, SEMI: semicolon in query, EQ: equal sign in query
the errors in the enum are ordered from least to most precedence -- higher ones will override lower ones
the term returned will always needs freeing regardless of whether there was an error or not
*/
enum { OK,OPEN,MCH,SEMI,EQ,PRN,EMPTY,END };
term parseTerms(char *s, int *er);

/*
takes a string, and a root to add the rule to
same error scheme as above, but with different meanings (if there was an error, root will not be changed)
PRN: unbalanced parens, SEMI: more or less than 1 semicolon in rule,
EQ: more or less than 1 equal sign in rule, EMPTY: match on nothing, END: semicolon is not at end of rule,
MCH: tried to match on quote
*/
int parseRule(char *s, rule root);

/*
takes a filename as a string, and a root to add rules to
returns a non-zero positive error on failure: one of the parseRule failure codes, or OPEN for failure to open file
if there is a rule in a rule, rules up until it will still be added to root
*/
int parseFile(char *name, rule root);

/*
exactly like the above, except reads the rules directly from a string
*/
int parseRules(char *string, rule root);

void rewrite(rule r, term t); // rewrites the terms `t` using the rules `r` until no more rewrites can be applied

/*
the same as above, except it always rewrites exactly once, which is the one rule with the most precedence
returns 1 if no more rewrites can be applied, and 0 if more can
*/
int stepRewrite(rule r, term t); 

/*
you might notice that some functions are missing - namely parseTerm, which parses a single term, and prettyRule, which pretty prints a single rule
parseTerm is missing because it is not needed
prettyRule is missing because in this rule layout, it would be very clumsy to make a function that prints a specific rule
this is the more feasable of the two functions to add, and I might add it in the future, but it's still largely useless
*/
#endif
