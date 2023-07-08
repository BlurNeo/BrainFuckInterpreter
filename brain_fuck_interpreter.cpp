#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

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
            // [,6
            case ('['):
                return JUMP_Z_F;
            // ],7
            case (']'):
                return JUMP_NZ_B;
            default:
                return -1;
        }
    }
};

class CodeMemory {
 public:
    CodeMemory() {
        code_segment_.reserve(1000);
    }
    std::vector<int> & get_code_segment() { return code_segment_; }
    void push_one_code(int code) {
        code_segment_.push_back(code);
    }
    int fetch_one_code() {
        return code_segment_[pc_pointer_++];
    }
    int get_pc_pointer() {
        return pc_pointer_;
    }
    void set_pointer_position(int position) {
        pc_pointer_ = position;
    }
    bool end() {
        return pc_pointer_ < 0 || pc_pointer_ >= code_segment_.size();
    }
    int get_jump_position(int pos) {
        return jump_map[pos];
    }
    void reset_pc_pointer() { pc_pointer_ = 0; }
    void record_jump() {
        int i = 0, j = code_segment_.size() - 1;
        while (i < j) {
            if (code_segment_[i] == '[') {
                while (code_segment_[j] != ']') {
                    j--;
                }
                jump_map[i] = j;
            }
        }
    }
 
 private:
    int pc_pointer_ = -1;
    std::vector<int> code_segment_;
    std::map<int, int> jump_map;
};

class StackMemory {
 public:
    StackMemory() {
        stack_segment_.resize(1000);
    }
    std::vector<int> & get_stack_segment() { return stack_segment_; }
    void increase_pointer() {
        stack_pointer_++;
    }
    void decrease_pointer() {
        stack_pointer_--;
    }
    void increase() {
        stack_segment_[stack_pointer_]++;
    }
    void decrease() {
        stack_segment_[stack_pointer_]--;
    }
    int get() {
        return stack_segment_[stack_pointer_];
    }
    void set(int val) {
        stack_segment_[stack_pointer_] = val;
    }
 private:
    int stack_pointer_ = 0;
    std::vector<int> stack_segment_;
};

class Interpreter;
class Machine {
 public:
    void run() {
        code_mem_.reset_pc_pointer();
        while (!code_mem_.end()) {
            int code = code_mem_.fetch_one_code();
            printf("code: %d\n", code);
            int pc, jump, ch;
            switch (code) {
                case Instruction::INC_PTR:
                    stk_mem_.increase_pointer();
                    break;
                case Instruction::DEC_PTR:
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
                    printf("Out:%d\n", ch);
                    break;
                case Instruction::INPUT_VAL:
                    break;
                case Instruction::JUMP_Z_F :
                {
                    if (stk_mem_.get() == 0) {
                        pc = code_mem_.get_pc_pointer();
                        jump = code_mem_.get_jump_position(pc);
                        code_mem_.set_pointer_position(jump + 1);
                    }
                    break;
                }
                case Instruction::JUMP_NZ_B :
                {
                    if (stk_mem_.get() != 0) {
                        pc = code_mem_.get_pc_pointer();
                        jump = code_mem_.get_jump_position(pc);
                        code_mem_.set_pointer_position(jump + 1);
                    }
                    break;
                }
            }
        }
    }

    friend Interpreter;
 private:
    CodeMemory code_mem_;
    StackMemory stk_mem_;
};

class Interpreter {
 public:
    Interpreter(Machine* machine) : machine_(machine) {}
    bool syntax_check(std::string_view code_str) {
        return true;
    }
    void interpret_from_string(std::string_view code_str) {
        syntax_check(code_str);
        for (auto ch : code_str) {
            int code = Instruction::from_char(ch);
            std::cout << "read code::" << code << "\n";
            machine_->code_mem_.push_one_code(code);
        }
    }
 private:
    Machine* machine_;
};
}

int main() {
    brainfuck::Machine machine;
    brainfuck::Interpreter interpreter(&machine);
    std::string code = "++++.+.+.";
    interpreter.interpret_from_string(code);
    machine.run();
}