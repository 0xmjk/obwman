
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
