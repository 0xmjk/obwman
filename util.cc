/* 
 * okon's bandwidth manager - Simple and efficient traffic shaping solution
 * Copyright (C) 2004-2007 Michal J. Kubski
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
