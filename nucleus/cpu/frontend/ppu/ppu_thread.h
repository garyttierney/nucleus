/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"
#include "nucleus/cpu/thread.h"

namespace cpu {
namespace frontend {
namespace ppu {

// Forward declarations
class PPUState;

class PPUThread : public Thread {
public:
    std::unique_ptr<PPUState> state;

    PPUThread(CPU* parent = nullptr);
    ~PPUThread();

    virtual void start() override;
    virtual void task() override;

    // Control the thread once it is started
    virtual void run() override;
    virtual void pause() override;
    virtual void stop() override;
};

}  // namespace ppu
}  // namespace frontend
}  // namespace cpu
