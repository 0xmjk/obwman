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
