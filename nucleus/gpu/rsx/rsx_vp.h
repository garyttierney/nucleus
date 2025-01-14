/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"

#include <string>

namespace gpu {

// RSX Vertex Program opcodes
enum {
    // Vertex Program VEC Opcodes
    RSX_VP_OPCODE_VEC_NOP = 0x00, // No-Operation
    RSX_VP_OPCODE_VEC_MOV = 0x01, // Move
    RSX_VP_OPCODE_VEC_MUL = 0x02, // Multiply
    RSX_VP_OPCODE_VEC_ADD = 0x03, // Addition
    RSX_VP_OPCODE_VEC_MAD = 0x04, // Multiply-Add
    RSX_VP_OPCODE_VEC_DP3 = 0x05, // 3-component Dot Product
    RSX_VP_OPCODE_VEC_DPH = 0x06, // Homogeneous Dot Product
    RSX_VP_OPCODE_VEC_DP4 = 0x07, // 4-component Dot Product
    RSX_VP_OPCODE_VEC_DST = 0x08, // Calculate distance vector
    RSX_VP_OPCODE_VEC_MIN = 0x09, // Minimum
    RSX_VP_OPCODE_VEC_MAX = 0x0A, // Maximum
    RSX_VP_OPCODE_VEC_SLT = 0x0B, // Set-If-LessThan
    RSX_VP_OPCODE_VEC_SGE = 0x0C, // Set-If-GreaterEqual
    RSX_VP_OPCODE_VEC_ARL = 0x0D, // Load to address register (round down)
    RSX_VP_OPCODE_VEC_FRC = 0x0E, // Extract fractional part (fraction)
    RSX_VP_OPCODE_VEC_FLR = 0x0F, // Round down (floor)
    RSX_VP_OPCODE_VEC_SEQ = 0x10, // Set-If-Equal
    RSX_VP_OPCODE_VEC_SFL = 0x11, // Set-If-False
    RSX_VP_OPCODE_VEC_SGT = 0x12, // Set-If-GreaterThan
    RSX_VP_OPCODE_VEC_SLE = 0x13, // Set-If-LessEqual
    RSX_VP_OPCODE_VEC_SNE = 0x14, // Set-If-NotEqual
    RSX_VP_OPCODE_VEC_STR = 0x15, // Set-If-True
    RSX_VP_OPCODE_VEC_SSG = 0x16, // Convert postive values to 1 and negative values to -1
    RSX_VP_OPCODE_VEC_TEX = 0x19, // Texture fetch

    // Vertex Program SCA Opcodes
    RSX_VP_OPCODE_SCA_NOP = 0x00, // No-Operation
    RSX_VP_OPCODE_SCA_MOV = 0x01, // Move (copy)
    RSX_VP_OPCODE_SCA_RCP = 0x02, // Reciprocal
    RSX_VP_OPCODE_SCA_RCC = 0x03, // Reciprocal clamped
    RSX_VP_OPCODE_SCA_RSQ = 0x04, // Reciprocal square root
    RSX_VP_OPCODE_SCA_EXP = 0x05, // Exponential base 2 (low-precision)
    RSX_VP_OPCODE_SCA_LOG = 0x06, // Logarithm base 2 (low-precision)
    RSX_VP_OPCODE_SCA_LIT = 0x07, // Lighting calculation
    RSX_VP_OPCODE_SCA_BRA = 0x08, // Branch
    RSX_VP_OPCODE_SCA_BRI = 0x09, // Branch by CC register
    RSX_VP_OPCODE_SCA_CAL = 0x0A, // Subroutine call
    RSX_VP_OPCODE_SCA_CLI = 0x0B, // Subroutine call by CC register
    RSX_VP_OPCODE_SCA_RET = 0x0C, // Return from subroutine
    RSX_VP_OPCODE_SCA_LG2 = 0x0D, // Logarithm base 2
    RSX_VP_OPCODE_SCA_EX2 = 0x0E, // Exponential base 2
    RSX_VP_OPCODE_SCA_SIN = 0x0F, // Sine function
    RSX_VP_OPCODE_SCA_COS = 0x10, // Cosine function
    RSX_VP_OPCODE_SCA_BRB = 0x11, // Branch by Boolean constant
    RSX_VP_OPCODE_SCA_CLB = 0x12, // Subroutine call by Boolean constant
    RSX_VP_OPCODE_SCA_PSH = 0x13, // Push onto stack
    RSX_VP_OPCODE_SCA_POP = 0x14, // Pop from stack
};

// RSX Vertex Program vector register
struct rsx_vp_constant_t {
    bool dirty = false; // Flag: Needs to be reuploaded
    union {
        U32 word[4];
        struct {
            F32 x;
            F32 y;
            F32 z;
            F32 w;
        };
    };
};

// RSX Vertex Program instruction
union rsx_vp_instruction_t
{
#define FIELD(from, to, type) struct{ U32:(32-to-1); type:(to-from+1); U32:from; }

    U32 word[4];
    U64 dword[2];

    struct {
        union {
            FIELD( 8,  8, U32 abs_src0);
            FIELD( 9,  9, U32 abs_src1);
            FIELD(10, 10, U32 abs_src2);
            FIELD(11, 16, U32 dst_data);
            FIELD(22, 29, U32 swizzling);
        };
        union {
            FIELD( 0,  4, U32 opcode_sca);
            FIELD( 5,  9, U32 opcode_vec);
            FIELD(10, 19, U32 src_const);
            FIELD(20, 23, U32 src_input);
            FIELD(24, 31, U32 src0_hi);
        };
        union {
            FIELD( 0,  8, U32 src0_lo);
            FIELD( 9, 25, U32 src1);
            FIELD(26, 31, U32 src2_hi);
        };
        union {
            FIELD( 0, 10, U32 src2_lo);
            FIELD(11, 14, U32 mask_sca);
            FIELD(15, 18, U32 masc_vec);
            FIELD(25, 29, U32 dst);
            FIELD(31, 31, U32 end);
        };
    };

#undef FIELD
};

union rsx_vp_instruction_source_t
{
    U32 value;

    struct {
        U32 type      : 2;
        U32 index     : 6;
        U32 swizzling : 8;
        U32 neg       : 1;
    };
};

class RSXVertexProgram {
    // Input/Output registers used in the program
    U32 usedInputs;
    U32 usedOutputs;

    // Current instruction being processed
    rsx_vp_instruction_t instr;

    // Generate the GLSL header and register declarations based on the decompilation
    std::string get_header();

    // Get the source and destination registers of the current instruction
    std::string get_src(U32 n);
    std::string get_dst();

public:
    // OpenGL shader ID
    U32 id = 0;

    // Generate a GLSL vertex shader equivalent to the VPE instruction buffer at the given start offset
    void decompile(rsx_vp_instruction_t* buffer);

    // Compile the generated GLSL code for the host GPU
    bool compile();
};

}  // namespace gpu
