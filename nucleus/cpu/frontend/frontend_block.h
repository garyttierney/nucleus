/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"

namespace cpu {
namespace frontend {

// Forward declarations
template <typename TAddr>
class Function;

template <typename TAddr>
class Block {
public:
    Function<TAddr>* parent;

    // Starting address
    TAddr address = 0;

    // Number of bytes covered by this block
    TAddr size = 0;

    // Branching
    TAddr branch_a = 0; // Conditional-True or Unconditional branching address
    TAddr branch_b = 0; // Conditional-False branching address

    // Check whether an address is inside this block
    bool contains(TAddr addr) const {
        const TAddr from = address;
        const TAddr to = address + size;
        return from <= addr && addr < to;
    }

    // Cut this block and return the remaining part as a new object
    Block<TAddr> split(TAddr cut) {
        // Configure new block
        Block<TAddr> new_block{};
        new_block.parent = parent;
        new_block.address = cut;
        new_block.size = size - (cut - address);
        new_block.branch_a = branch_a;
        new_block.branch_b = branch_b;

        // Update this block
        size = cut - address;
        branch_a = cut;
        branch_b = 0;

        return new_block;
    }
};

}  // namespace frontend
}  // namespace cpu
