#include <fstream>
#include <string>
#include <errno.h>
#include <string.h>
#include <iostream>
#include "config.h"
#include "util.h"

using namespace std;

void obwman::LOG(string buffer)
{
        ofstream outFile(OBW_LOG_FILE,
                         std::ios_base::out | std::ios_base::app);
    	if (!outFile) {
        	cerr << "Unable to open the log file (" << OBW_LOG_FILE << "): " << strerror(errno) << endl;
        	exit (errno);  
    	}
        outFile << buffer << endl;
        outFile.close();
}
