#include "brain_fuck/interpreter.h"
#include "brain_fuck/machine.h"

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