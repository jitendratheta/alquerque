//
//  timer.hpp
//  Alquerque
//
//  Created by Jitendra S. Kushwaha on 16/02/17.
//  Copyright © 2017 Jitendra S. Kushwaha. All rights reserved.
//

#ifndef timer_hpp
#define timer_hpp

#include <sys/time.h>
#include <time.h>
#include "types.h"

class Timer {

    timeval mBaseTime;
    double mDuration;
    double timeDiff();
    
public:
    void reset();
    void start(double duration);
    bool isTimeUp();
};

extern Timer timer;

#endif /* timer_hpp */
