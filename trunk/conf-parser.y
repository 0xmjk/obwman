%{
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

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

using namespace obwman;

int		lineno	= 1;	/* number of current source line */
extern int	yylex();	/* lexical analyzer generated from lex.l */
extern char	*yytext;	/* last token, defined in lex.l  */

void
yyerror(parser &p, char *s)
{
    fprintf(stderr,"Syntax error on line #%d: %s\n",lineno,s);
    fprintf(stderr,"Last token was \"%s\"\n",yytext);
    exit(1);
}

%}

%union {
    int ival;
    char *sval;
    }

%type <sval> STRING IPADDRESS
%type <ival> VALUE

%parse-param {parser& p}

%token DEF EXCEPT OMIT IPADDRESS STRING VALUE NL SLASH HOST PORT
%%

program	    : def_block second_block omit_block ports_block
	    ;

def_block : definition def_block
	  | definition
	  ;

	    
second_block : hosts_block except_block
	     | hosts_block
	     | except_block
	     |
	     ;

except_block: except except_block
	    | except
	      ;

hosts_block: hosts hosts_block
	    | hosts
	     ;

ports_block: ports ports_block
	    |
	     ;

omit_block: omit omit_block
	    |
	     ;

definition  : DEF STRING IPADDRESS { p.definition_ss($2, $3); }
	    | DEF STRING STRING  { p.definition_ss($2, $3); }
	    | DEF STRING VALUE { p.definition_si($2, $3); }
	    ;
	    
except : EXCEPT IPADDRESS VALUE VALUE VALUE VALUE { p.exception($2, $3, $4, $5, $6); }
	    ;
	    
omit   : OMIT IPADDRESS SLASH VALUE { p.omission($2, $4); }
	    | OMIT IPADDRESS { p.omission($2, 32); }
	    ;

ports  : PORT VALUE { p.port($2); }
	    ;

hosts  : HOST IPADDRESS { p.host($2); }
	    ;

%%


		
