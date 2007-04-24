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
  
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <algorithm>
#include <map>
#include <vector>

#include "config.h"

#include "control_parameters.h"
#include "configuration.h"
#include "parser.h"


using namespace std;
using namespace obwman;

configuration::configuration()
{

}

void configuration::load() {
    FILE *cfg = fopen(OBW_CONF_FILE, "r");
    if (!cfg) {
    	cerr << "Unable to open the configuration file (" << OBW_CONF_FILE << "): " << strerror(errno) << endl;
	exit (errno);
    }
    parser p(this);
    yyin = cfg;
    yyparse(p);
    fclose (cfg);
    populate_string_variables();
    populate_integer_variables();
    cerr << "Configuration initalized..." << endl;
}

void configuration::populate_string_variables()
{
    ip = string_variables["ip"];
    lan = string_variables["lan"];
    speed_unit = string_variables["speed_unit"];
}

void configuration::populate_integer_variables()
{
    defaults.down_rate = integer_variables["down_rate"];
    defaults.up_rate = integer_variables["up_rate"];
    defaults.down_ceil = integer_variables["down_ceil"];
    defaults.up_ceil = integer_variables["up_ceil"];
    lan_speed = integer_variables["lan_speed"];
    down_speed = integer_variables["down_speed"];
    up_speed = integer_variables["up_speed"];
    up_burst = integer_variables["up_burst"];
    down_burst = integer_variables["down_burst"];
    squid_port = integer_variables["squid_port"];
}
