/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"

namespace cpu {
namespace frontend {
namespace ppu {

// PPU Instruction
union Instruction
{
#define FIELD(from, to, type) struct{ U32:(32-to-1); type:(to-from+1); U32:from; }

    /**
     * PPU Instruction fields:
     * Represents the bit fields contained in 32-bit PPU instructions.
     */

    U32 value;

    // Opcode fields
    FIELD( 0,  5, U32 opcode);  // Primary opcode
    FIELD(26, 31, U32 op4);     // Extended opcode of 6-bits (up to 0x3F)
    FIELD(21, 31, U32 op4_);    // Extended opcode of 11-bits (up to 0x7FF)
    FIELD(21, 30, U32 op19);    // Extended opcode of 10-bits (up to 0x3FF)
    FIELD(27, 29, U32 op30);    // Extended opcode of 3-bits (up to 0x7)
    FIELD(21, 30, U32 op31);    // Extended opcode of 10-bits (up to 0x3FF)
    FIELD(30, 31, U32 op58);    // Extended opcode of 2-bits (up to 0x3)
    FIELD(26, 30, U32 op59);    // Extended opcode of 5-bits (up to 0x1F)
    FIELD(30, 31, U32 op62);    // Extended opcode of 2-bits (up to 0x3)
    FIELD(26, 30, U32 op63);    // Extended opcode of 5-bits (up to 0x1F)
    FIELD(21, 30, U32 op63_);   // Extended opcode of 10-bits (up to 0x3FF)

    // Instruction fields
    FIELD(30, 30, U32 aa);      // Bit/Flags: Absolute address bit
    FIELD(31, 31, U32 lk);      // Bit/Flags: Link bit: Update the link register (LR)
    FIELD(21, 21, U32 oe);      // Bit/Flags: OE bit: Enable enable setting OV and SO in the XER
    FIELD(31, 31, U32 rc);      // Bit/Flags: Record bit: Update the condition register (CR)
    FIELD( 6,  6, U32 l6);      // Bit/Flags: ?
    FIELD(10, 10, U32 l10);     // Bit/Flags: ?
    FIELD(11, 11, U32 l11);     // Bit/Flags: ?
    FIELD( 9, 10, U32 l9_10);   // Bit/Flags: ?
    FIELD( 6, 10, U32 bo);      // Branching: Options for the branch conditional instructions
    FIELD(11, 15, U32 bi);      // Branching: CR bit to trigger branch conditional instructions
    FIELD(16, 29, S32 bd);      // Branching: Immediate 14-bit signed integer for branch displacement
    FIELD(19, 20, U32 bh);      // ?
    FIELD(11, 13, U32 bfa);     // ?
    FIELD( 6,  8, U32 crfd);    // CR fields: Destination CR or FPSCR field
    FIELD(11, 13, U32 crfs);    // CR fields: Source CR or FPSCR field
    FIELD( 6, 10, U32 crbd);    // CR fields: Destination bit in the CR or FPSCR
    FIELD(11, 15, U32 crba);    // CR fields: Source bit in the CR
    FIELD(16, 20, U32 crbb);    // CR fields: Source bit in the CR
    FIELD(12, 19, U32 crm);     // Identify the CR fields that are to be updated by the mtcrf instruction
    FIELD(16, 31, S32 d);       // Immediate 16-bit signed integer
    FIELD(16, 27, U32 dq);      // ?
    FIELD(16, 29, S32 ds);      // ?
    FIELD( 7, 14, U32 fm);      // ?
    FIELD( 6, 10, U32 frd);     // FPR: Destination
    FIELD( 6, 10, U32 frs);     // FPR: Source
    FIELD(11, 15, U32 fra);     // FPR: Source
    FIELD(16, 20, U32 frb);     // FPR: Source
    FIELD(21, 25, U32 frc);     // FPR: Source
    FIELD(16, 19, U32 imm);     // Immediate for to place in FPSCR
    FIELD( 6, 29, S32 li);      // Branching:
    FIELD( 6, 29, S32 ll);      // Branching:
    FIELD(21, 25, U32 mb);      // First '1' bit of a 64-bit mask in rotate instructions
    FIELD(26, 26, U32 mb_);     // First '1' bit of a 64-bit mask in rotate instructions: Split field
    FIELD(26, 30, U32 me);      // Last '1' bit of a 64-bit mask in rotate instructions
    FIELD(21, 25, U32 me_);     // Last '1' bit of a 64-bit mask in rotate instructions: Split field
    FIELD(26, 26, U32 me__);    // Last '1' bit of a 64-bit mask in rotate instructions: Split field
    FIELD(16, 20, U32 nb);      // Number of bytes to move in an immediate string load or store
    FIELD( 6, 10, U32 rd);      // GPR: Destination
    FIELD( 6, 10, U32 rs);      // GPR: Source
    FIELD(11, 15, U32 ra);      // GPR: Source
    FIELD(16, 20, U32 rb);      // GPR: Source
    FIELD(16, 20, U32 sh);      // Shift amount
    FIELD(30, 30, U32 sh_);     // Shift amount: Split field
    FIELD(11, 20, U32 spr);     // Special-purpose register
    FIELD( 9, 10, U32 strm);    // ?
    FIELD(20, 26, U32 lev);     // ?
    FIELD(16, 31, S32 simm);    // Immediate 16-bit signed integer
    FIELD(16, 31, U32 uimm);    // Immediate 16-bit unsigned integer
    FIELD( 9, 10, U32 th);      // Data stream variant of the dcbt instruction
    FIELD( 6, 10, U32 to);      // Trap conditions
    FIELD( 6, 10, U32 vd);      // Vector/SIMD: Destination vector register
    FIELD( 6, 10, U32 vs);      // Vector/SIMD: Source vector register
    FIELD(11, 15, U32 va);      // Vector/SIMD: Source vector register
    FIELD(16, 20, U32 vb);      // Vector/SIMD: Source vector register
    FIELD(21, 25, U32 vc);      // Vector/SIMD: Source vector register
    FIELD(22, 25, U32 vshb);    // Vector/SIMD: Specifies a shift amount in bytes
    FIELD(11, 15, S32 vsimm);   // Vector/SIMD: Immediate 5-bit signed integer
    FIELD(11, 15, U32 vuimm);   // Vector/SIMD: Immediate 5-bit unsigned integer

    /**
     * PPU Instruction properties:
     * Properties of the current instruction, required by the analyzer.
     */

    // Determines whether the instruction is a valid PPU instruction
    bool is_valid() const;

    // Determines whether the instruction is branch instruction
    bool is_branch() const;
    bool is_branch_conditional() const;
    bool is_branch_unconditional() const;

    // Determines whether the instruction is function call instruction
    bool is_call() const;
    bool is_call_known() const;   // Calling address can be predicted
    bool is_call_unknown() const; // Calling address cannot be predicted
    bool is_jump() const;
    bool is_jump_known() const;   // Jumping address can be predicted
    bool is_jump_unknown() const; // Jumping address cannot be predicted

    // Determines whether the instruction is return instruction
    bool is_return() const;

    // Obtain the target address if the branch is taken
    U32 get_target(U32 currentAddr) const;

#undef FIELD
};

}  // namespace ppu
}  // namespace frontend
}  // namespace cpu
