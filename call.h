#ifndef __OBWMAN_CALL_H
#define __OBWMAN_CALL_H

#include <string> 

using namespace std;

namespace obwman {

class call {

public:

call();

void iptables_atomic(string buffer);
int system(const char *cmd);
int system(string p);
int ip(string p);
int iptables(string p);
int tc(string p);

};

}
#endif /* __CALL_H */
