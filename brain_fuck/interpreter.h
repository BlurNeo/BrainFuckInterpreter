#pragma once

#include <string>
#include <iostream>
#include "machine.h"
#include "instruction.h"

namespace brainfuck {

class Interpreter {
 public:
    Interpreter(Machine* machine) : machine_(machine) {}
    bool syntax_check(std::string_view code_str) {
        return true;
    }
    void interpret_from_string(std::string_view code_str) {
        machine_->reset();
        syntax_check(code_str);
        for (auto ch : code_str) {
            int code = Instruction::from_char(ch);
            // std::cout << "read code::" << code << "\n";
            machine_->code_mem_.push_one_code(code);
        }
        machine_->code_mem_.record_jump();
    }
 private:
    Machine* machine_;
};

}