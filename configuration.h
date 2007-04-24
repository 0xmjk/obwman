/* 
 * okon's bandwidth manager - Simple and efficient traffic shaping solution
 * Copyright (C) 2004-2006 Michal J. Kubski
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

    configuration();
    
    void load();

    private:
        void populate_string_variables();
	void populate_integer_variables();                                             

};

};

#endif
