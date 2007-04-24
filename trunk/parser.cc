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

#include <string>
#include <map>
#include "parser.h"
#include "configuration.h"
#include "util.h"

using namespace std;
using namespace obwman;

parser::parser(configuration *conf) {
    this->conf = conf;
}


void parser::definition_ss(char *variable, char *value)
{
    configuration::string_variable v(variable, value);
    conf->string_variables.insert(v);    
}

void parser::definition_si(char *variable, int value)
{
    configuration::integer_variable v(variable, value);
    conf->integer_variables.insert(v);    
}

void parser::exception(char *ip, int a, int b, int c, int d)
{
    control_parameters par(a,b,c,d);
    configuration::host_definition h(ip, par);
    conf->ip_list.erase(ip);
    conf->ip_list.insert(h);    
}

void parser::omission(char *ip, int prefix)
{
    string s(ip);
    
    s = s + "/" + to_string(prefix);

    conf->ip_omit_list.push_back(s);
}

void parser::port(int port)
{
    conf->local_ports_list.push_back(to_string(port));
}

void parser::host(char *ip)
{
    configuration::host_definition h(ip, conf->defaults);
    conf->ip_list.erase(ip);
    conf->ip_list.insert(h);    
}
