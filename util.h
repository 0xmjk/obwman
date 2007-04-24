
#ifndef __OBWMAN_UTIL_H
#define __OBWMAN_UTIL_H

#include <sstream>
#include <fstream>
#include <string>

using namespace std;

namespace obwman {
                              
template < class type > inline std::string to_string(const type & value)
{
    std::ostringstream streamOut;
    streamOut << value;
    return streamOut.str();
}


void LOG(string buffer);


};
	    
#endif /* __OBWMAN_UTIL_H */
