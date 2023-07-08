#pragma once

#include "instruction.h"
#include "memory.h"

namespace brainfuck {

class Interpreter;
class Machine {
 public:
    void reset() {
        code_mem_.reset();
        stk_mem_.reset();
    }
    void run() {
        code_mem_.reset_pc_pointer();
        while (!code_mem_.end()) {
            int code = code_mem_.fetch_one_code();
            // printf("code: %d\n", code);
            // printf("code ptr: %d\n", code_mem_.get_pc_pointer());
            // printf("Stk ptr: %d\n", stk_mem_.get_pointer());
            int pc, jump, ch;
            bool do_jump = false;
            switch (code) {
                case Instruction::INC_PTR:
                // printf("Inc Stk Ptr\n");
                    stk_mem_.increase_pointer();
                    break;
                case Instruction::DEC_PTR:
                    // printf("Stk ptr: %d\n", stk_mem_.get_pointer());
                    stk_mem_.decrease_pointer();
                    break;
                case Instruction::INC_VAL:
                    stk_mem_.increase();
                    break;
                case Instruction::DEC_VAL:
                    stk_mem_.decrease();
                    break;
                case Instruction::OUTPUT_VAL:
                    ch = (char)stk_mem_.get();
                    // printf("Stk ptr: %d\n", stk_mem_.get_pointer());
                    printf("%c", ch);
                    break;
                case Instruction::INPUT_VAL:
                    break;
                case Instruction::JUMP_Z_F :
                {
                    if (stk_mem_.get() == 0) {
                        pc = code_mem_.get_pc_pointer();
                        jump = code_mem_.get_f_jump_position(pc);
                        code_mem_.set_pointer_position(jump + 1);
                        do_jump = true;
                    }
                    break;
                }
                case Instruction::JUMP_NZ_B :
                {
                    if (stk_mem_.get() != 0) {
                        pc = code_mem_.get_pc_pointer();
                        jump = code_mem_.get_b_jump_position(pc);
                        // std::cout << "pc,jump:" <<pc<<","<<jump << "\n";
                        code_mem_.set_pointer_position(jump + 1);
                        do_jump = true;
                    }
                    break;
                }
            }
            if (!do_jump) {
                code_mem_.increase_pc_pointer();
            }
            // sleep(1);
        }
    }

    friend Interpreter;
 private:
    CodeMemory code_mem_;
    StackMemory stk_mem_;
};

}