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
