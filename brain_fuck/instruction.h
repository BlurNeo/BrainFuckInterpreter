#pragma once

namespace brainfuck {

class Instruction {
 public:
    enum {
        INC_PTR = 0,
        DEC_PTR = 1,
        INC_VAL = 2,
        DEC_VAL = 3,
        OUTPUT_VAL = 4,
        INPUT_VAL = 5,
        JUMP_Z_F = 6,
        JUMP_NZ_B = 7
    } INST_CODE;
    
    static int from_char(char ch) {
        switch(ch) {
            // >:0
            case ('>'):
                return INC_PTR;
            // <:1
            case ('<'):
                return DEC_PTR;
            // +:2
            case ('+'):
                return INC_VAL;
            // -:3
            case ('-'):
                return DEC_VAL;
            // .:4
            case ('.'):
                return OUTPUT_VAL;
            // ,:5
            case (','):
                return INPUT_VAL;
            // [:6
            case ('['):
                return JUMP_Z_F;
            // ]:7
            case (']'):
                return JUMP_NZ_B;
            default:
                return -1;
        }
    }
};

}