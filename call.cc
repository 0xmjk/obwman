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
//	this->system("rm " + filename_str);
}

int call::system(const char *cmd) {
	LOG(cmd);
	return std::system(cmd);
}

int call::system(string p) {
    string r =  p + " >>" OBW_LOG_FILE " 2>&1";
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
