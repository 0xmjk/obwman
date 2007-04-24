/*
 * Lexer for Obwman
 */

%{

#include <string.h>
#include <stdio.h>

#include "conf-parser.h"

extern int lineno;

%}

DIGIT		[0-9]
STRINGCHARS	[a-zA-Z0-9_]
IPOCTET	[0-9]{1,3}
IPADDRESS	({IPOCTET}"."){3}{IPOCTET}
VALUE	[0-9]+
STRING {STRINGCHARS}+
COMMENT	(\#.*)$

%%

[ \t\r]	break;
{COMMENT} break;

\n	{ lineno++; break; }
"/"	{ return SLASH; }

{IPADDRESS}	{ yylval.sval=strdup(yytext); return IPADDRESS;}
{VALUE} { yylval.ival=atoi(yytext);  return VALUE; }
def	{ return DEF; }
except	{ return EXCEPT; }
omit 	{ return OMIT; } 
host 	{ return HOST; } 
port 	{ return PORT; } 
{STRING} {yylval.sval=strdup(yytext); return STRING; }
.			{ fprintf(stderr, "wrong character at %d line \n",lineno); }

%%

int
yywrap()
{
return 1; /* tell scanner no further files need to be processed */
}
