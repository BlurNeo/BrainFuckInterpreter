#pragma once
#include <vector>
#include <stack>
#include <map>
#include "instruction.h"

namespace brainfuck {

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

}