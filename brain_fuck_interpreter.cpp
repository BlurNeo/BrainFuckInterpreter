#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stack>

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

class CodeMemory {
 public:
    CodeMemory() {
        code_segment_.reserve(1000);
    }
    void reset() {
        code_segment_.clear();
        code_segment_.reserve(1000);
        reset_pc_pointer();
        f_jump_map.clear();
        b_jump_map.clear();
    }
    std::vector<int> & get_code_segment() { return code_segment_; }
    void push_one_code(int code) {
        code_segment_.push_back(code);
    }
    int fetch_one_code() {
        return code_segment_[pc_pointer_];
    }
    void increase_pc_pointer() {
        pc_pointer_++;
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
    int get_f_jump_position(int pos) {
        return f_jump_map[pos];
    }
    int get_b_jump_position(int pos) {
        return b_jump_map[pos];
    }
    void reset_pc_pointer() { pc_pointer_ = 0; }
    void record_jump() {
        std::stack<int> tmp;
        for (int i = 0; i < code_segment_.size(); i++) {
            if (code_segment_[i] == Instruction::JUMP_Z_F) {
                tmp.push(i);
            } else if (code_segment_[i] == Instruction::JUMP_NZ_B) {
                // std::cout << "stk size:" << tmp.size();
                int pos = tmp.top();
                f_jump_map[pos] = i;
                b_jump_map[i] = pos;
                tmp.pop();
            }
        }
        // std::cout << "record jump:\n";
        // for (auto pair : f_jump_map) {
        //     std::cout << "i,j:" << pair.first <<","<<pair.second<<"\n";
        // }
        // for (auto pair : b_jump_map) {
        //     std::cout << "i,j:" << pair.first <<","<<pair.second<<"\n";
        // }
    }
 
 private:
    int pc_pointer_ = -1;
    std::vector<int> code_segment_;
    std::map<int, int> f_jump_map;
    std::map<int, int> b_jump_map;
};

class StackMemory {
 public:
    StackMemory() {
        stack_segment_.resize(1000);
    }
    void reset() {
        stack_segment_.clear();
        stack_segment_.resize(1000);
        stack_pointer_ = 0;
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
    int get_pointer() { return stack_pointer_; }
 private:
    int stack_pointer_ = 0;
    std::vector<int> stack_segment_;
};

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

int main() {
    brainfuck::Machine machine;
    brainfuck::Interpreter interpreter(&machine);
    std::string code_add = "++>+++++[<+>-]++++++++[<++++++>-]<.";
    std::string code_helloworld = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    std::cout << "=========>2+5:\n";
    interpreter.interpret_from_string(code_add);
    machine.run();
    std::cout << "\n=========>print helloworld:\n";
    interpreter.interpret_from_string(code_helloworld);
    machine.run();
}