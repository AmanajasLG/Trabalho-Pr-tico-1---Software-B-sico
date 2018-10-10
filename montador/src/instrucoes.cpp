#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <../include/boost_1_68_0/boost/algorithm/string.hpp>

class Instructions
{

    struct Instruction
    {
        std::string name;
        int code;
        int size;
    };

  public:
    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - código - tamanho
    */
    bool isInstruction()
    {
    }

    void readInstructionFile()
    {
        std::ifstream file;
        file.open("instrucoes.txt");
    }

  private:
    std::vector<Instruction> _instructon;
};
