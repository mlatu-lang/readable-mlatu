#ifndef mlatu_h
#define mlatu_h

// type, word, children, next
typedef struct t { int t; char *w; struct t *c,*n; } *T, *term;
void freeTerms(term m);
char *prettyTerms(term t); // pretty prints a list of terms, returning the allocated string
char *prettyTerm(term t); // pretty prints a single term, returning the allocated string

// word, children, next, rewrite, total length length (of rule + parents), rewrite to empty term?
typedef struct d { char *w; struct d *c, *n; term r; int l, e; } *D, *rule; // definition (R was taken)
/*  rule matching layout
	mlatu: foo = x; foo bar = y; foo baz = z;
	internal representation:
    foo = x
       |
      ->c 
    bar = y  ->n  baz = z 
	rules are represented as a tree, where, in this example, the word foo is the parent of bar and baz
	parentship is represented by foo's child pointer pointing to bar, while bar's next pointer points to baz
	this means that if you can't match foo, it doesn't even try foo bar or foo baz
	you can see the tree in the repl by typing )ruletree */
rule newRoot();
void freeRules(rule r);

/*
takes a string, and a pointer to an error variable
if the parse was successful, er will be 0 (OK)
otherwise, it will be nonzero -- one of
PRD: unbalanced parens, PRD: period in query, EQ: equal sign in query
the errors in the enum are ordered from least to most precedence -- higher ones will override lower ones
the term returned will always needs freeing regardless of whether there was an error or not
*/
enum { OK,OPEN,MCH,PRD,EQ,PRN,EMPTY,END };
term parseTerms(char *s, int *er);

/*
takes a string, and a root to add the rule to
same error scheme as above, but with different meanings (if there was an error, root will not be changed)
PRD: unbalanced parens, PRD: more or less than 1 period in rule,
EQ: more or less than 1 equal sign in rule, EMPTY: match on nothing, END: period is not at end of rule,
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

/*
rewrites the terms `t` using the rules `r` until no more rewrites can be applied
returns the number of rewrites
*/
int rewrite(rule r, term t);

/*
rewrites terms `t` with the rules `r` exactly once, applying the rule with the most precedence
returns 1 if no more rewrites can be applied, and 0 if more can
*/
int stepRewrite(rule r, term t); 

/*
you might notice that some functions are missing - namely parseTerm, which parses a single term, prettyRule, which pretty prints a single rule, and prettyRules, which pretty prints multiple rules
parseTerm is missing because tokenizing and parsing are done in one step, and aren't separate
prettyRule(s) is missing because in this tree-like rule layout, it doesn't make sense to have any sort of print function that prints one specific rule
*/
#endif
