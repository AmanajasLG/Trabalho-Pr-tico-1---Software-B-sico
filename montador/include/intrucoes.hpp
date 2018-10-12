#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

class Instructions
{

    struct Instruction
    {
        std::string name;
        int op_number;
        int op_code;
        int size;
    };

  public:
    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - código - tamanho
    */
    bool isInstruction();

    void readInstructionFile();

  private:
    std::vector<Instruction> _instruction;
};
