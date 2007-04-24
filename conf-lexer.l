/* 
 * okon's bandwidth manager - Simple and efficient traffic shaping solution
 * Copyright (C) 2004-2007 Michal J. Kubski
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; version 2 of the License. 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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
