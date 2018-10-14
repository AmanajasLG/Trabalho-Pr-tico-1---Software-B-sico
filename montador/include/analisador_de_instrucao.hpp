#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

class InstructionAnalizer
{
    struct Directive
    {
        std::string name;
        int op_number;
        int size;
    };

    struct Instruction
    {
        std::string name;
        int op_number;
        int op_code;
        int size;
    };

  public:
    InstructionAnalizer()
    {
        readDirectiveFile();
        readInstructionFile();
    }
    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - tamanho
    */
    bool isDirective(std::string label);

    void readDirectiveFile();

    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - código - tamanho
    */
    bool isInstruction(std::string label);

    int instructionOpNuber(std::string label);

    void readInstructionFile();

    bool isLable(std::string lable)
    {
        if (lable.back() == ':')
            return true;

        return false;
    }

    bool isComment(std::string lable)
    {
        if (lable.front() == ';')
            return true;

        return false;
    }

  private:
    std::vector<Directive> _directive;
    std::vector<Instruction> _instruction;
};