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

#include "control_parameters.h"

using namespace obwman;

control_parameters::control_parameters()
{
            down_rate = 0;
            up_rate = 0;
            down_ceil = 0;
            up_ceil = 0;
}

control_parameters::control_parameters(int _down_rate, int _up_rate,
					int _down_ceil, int _up_ceil) {
            down_rate = _down_rate;
            up_rate = _up_rate;
            down_ceil = _down_ceil;
            up_ceil = _up_ceil;
}

void control_parameters::put(control_parameters a) {
            down_rate = a.down_rate;
            up_rate = a.up_rate;
            down_ceil = a.down_ceil;
            up_ceil = a.up_ceil;
}

