
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

