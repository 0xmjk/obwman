
#ifndef __OBWMAN_CONFIGURATION_H

#define __OBWMAN_CONFIGURATION_H

#include <stdio.h>

#include <string>
#include <algorithm>
#include <map>
#include <vector>

#include "control_parameters.h"

using namespace std;

namespace obwman {

class configuration {

public:
    typedef pair<string, control_parameters> host_definition;
    typedef pair<string, string> string_variable;
    typedef pair<string, int> integer_variable;

    control_parameters defaults;
    map<string, control_parameters> ip_list;
    vector<string> ip_omit_list;
    vector<string> local_ports_list;    

    map<string, string> string_variables;
    map<string, int> integer_variables;

    string ip;
    int squid_port;
    string lan;
    string speed_unit;
    int lan_speed;
    int down_speed;
    int up_speed;
    int up_burst;
    int down_burst;    
    int up_cburst;
    int down_cburst;    

    configuration();
    
    void load();

    private:
        void populate_string_variables();
	void populate_integer_variables();                                             

};

};

#endif
