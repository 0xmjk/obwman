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

#include <string> 
#include <iostream> 
#include <fstream>
#include <errno.h>
#include <string.h>

#include "config.h"
#include "call.h"
#include "util.h"

using namespace obwman;
using namespace std;

call::call()
{

}

void call::iptables_atomic(string buffer)
{
	char filename[] = OBW_TMP_FILE;
 	mkstemp((char *) filename);
	ofstream outFile(filename,
			 std::ios_base::out | std::ios_base::app);
	if (!outFile) {
        	cerr << "Unable to open " << filename << ": " << strerror(errno) << endl;
        	exit (errno);  
    	}

	outFile << buffer << endl;
	outFile.close();
	string filename_str = filename;
	this->system("iptables-restore --noflush < " + filename_str);
	this->system("rm " + filename_str);
}

int call::system(const char *cmd) {
	LOG(cmd);
	return std::system(cmd);
}

int call::system(string p) {
    string r =  p + " &> /dev/null";
    return this->system(r.c_str());
}

int call::ip(string p) {
    return system("ip " + p);
}

int call::iptables(string p) {
    usleep (500000);
    return system("iptables " + p);
}

int call::tc(string p) {
    return system("tc " + p);
}
