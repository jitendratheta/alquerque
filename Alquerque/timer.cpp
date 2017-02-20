//
//  timer.cpp
//  Alquerque
//
//  Created by Jitendra S. Kushwaha on 16/02/17.
//  Copyright © 2017 Jitendra S. Kushwaha. All rights reserved.
//

#include "timer.hpp"
#include "constant.h"

double Timer::timeDiff() {
    timeval val;
    gettimeofday(&val, 0);
    return  (val.tv_sec - mBaseTime.tv_sec) * 1000  +
            (val.tv_usec - mBaseTime.tv_usec) / 1000;
}

void Timer::reset() {
    gettimeofday(&mBaseTime, 0);
}

/*
 * Start timer for given millisecond duration
 */
void Timer::start(double duration) {
    mDuration = duration;
    reset();
}

/*
 * Check of the timer is up after it was started
 */
bool Timer::isTimeUp() {
    DB2(timeDiff(), mDuration);
    return timeDiff() >= mDuration;
}
