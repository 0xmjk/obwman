
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
