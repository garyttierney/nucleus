/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "sys_timer.h"
#include "nucleus/system/lv2.h"

namespace sys {

S32 sys_timer_sleep(U32 sleep_time) {
    // TODO: Use a condition variable to kill the thread while it sleeps
    std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    return CELL_OK;
}

S32 sys_timer_usleep(U64 sleep_time) {
    // Maximum value is: 2^48-1
    if (sleep_time > 0xFFFFFFFFFFFFULL) {
        sleep_time = 0xFFFFFFFFFFFFULL;
    }
    // TODO: Use a condition variable to kill the thread while it sleeps
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    return CELL_OK;
}

}  // namespace sys
