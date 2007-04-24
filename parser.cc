
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
