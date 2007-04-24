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

#ifndef __OBWMAN_CONTROL_PARAMETERES_H

#define __OBWMAN_CONTROL_PARAMETERES_H

namespace obwman {

class control_parameters {

public:
    int down_rate;
    int up_rate;
    int down_ceil;
    int up_ceil;
                  

    control_parameters();
    control_parameters(int _down_rate, int _up_rate,
					int _down_ceil, int _up_ceil);

    void put(control_parameters a);

};

};

#endif
