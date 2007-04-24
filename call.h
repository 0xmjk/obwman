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
