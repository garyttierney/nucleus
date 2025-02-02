/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "sys_cond.h"
#include "sys_mutex.h"
#include "nucleus/emulator.h"
#include "nucleus/logger/logger.h"
#include "nucleus/system/lv2.h"

namespace sys {

S32 sys_cond_create(BE<U32>* cond_id, U32 mutex_id, sys_cond_attribute_t* attr) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    auto* mutex = lv2.objects.get<sys_mutex_t>(mutex_id);

    // Check requisites
    if (!mutex) {
        return CELL_ESRCH;
    }
    if (cond_id == nucleus.memory->ptr(0) || attr == nucleus.memory->ptr(0)) {
        return CELL_EFAULT;
    }
    if (attr->pshared != SYS_SYNC_PROCESS_SHARED && attr->pshared != SYS_SYNC_NOT_PROCESS_SHARED) {
        return CELL_EINVAL;
    }

    // Check for process-shareable flag
    if (attr->pshared == SYS_SYNC_PROCESS_SHARED) {
        logger.warning(LOG_HLE, "Process-shareable semaphores are not supported");
    }

    // Create condition variable
    auto* cond = new sys_cond_t();
    cond->mutex = mutex;
    cond->attr = *attr;

    *cond_id = lv2.objects.add(cond, SYS_COND_OBJECT);
    return CELL_OK;
}

S32 sys_cond_destroy(U32 cond_id) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    if (!lv2.objects.remove(cond_id)) {
        return CELL_ESRCH;
    }
    return CELL_OK;
}

S32 sys_cond_signal(U32 cond_id) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    auto* cond = lv2.objects.get<sys_cond_t>(cond_id);

    // Check requisites
    if (!cond) {
        return CELL_ESRCH;
    }

    cond->cv.notify_one();
    return CELL_OK;
}

S32 sys_cond_signal_all(U32 cond_id) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    auto* cond = lv2.objects.get<sys_cond_t>(cond_id);

    // Check requisites
    if (!cond) {
        return CELL_ESRCH;
    }

    cond->cv.notify_all();
    return CELL_OK;
}

S32 sys_cond_signal_to(U32 cond_id, U32 thread_id) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    auto* cond = lv2.objects.get<sys_cond_t>(cond_id);

    // Check requisites
    if (!cond) {
        return CELL_ESRCH;
    }

    // TODO: This is wrong
    cond->cv.notify_all();
    return CELL_OK;
}

S32 sys_cond_wait(U32 cond_id, U64 timeout) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    auto* cond = lv2.objects.get<sys_cond_t>(cond_id);

    // Check requisites
    if (!cond) {
        return CELL_ESRCH;
    }

    // Maximum value is: 2^48-1
    if (timeout > 0xFFFFFFFFFFFFULL) {
        timeout = 0xFFFFFFFFFFFFULL;
    }

    std::unique_lock<std::mutex> lock((std::mutex&)cond->mutex->mutex);
    if (timeout == 0) {
        cond->cv.wait(lock);
    } else {
        auto rel_time = std::chrono::microseconds(timeout);
        cond->cv.wait_for(lock, rel_time);
    }

    return CELL_OK;
}

}  // namespace sys
