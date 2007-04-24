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

#ifndef __OBWMAN_PARSER_H
#define __OBWMAN_PARSER_H

#include "configuration.h"


namespace obwman {

class parser {

  private:
    configuration *conf;

  public:
    parser(configuration *conf);
    
    void definition_ss(char *variable, char *value);
    void definition_si(char *variable, int value);
    void exception(char *ip, int a, int b, int c, int d);
    void omission(char *ip, int prefix);
    void port(int port);
    void host(char *ip);

};


}

using namespace obwman;

extern FILE* yyin;
extern int yyparse(parser& p);

#endif /* __PARSER_H */
