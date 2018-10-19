#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

#ifndef INSTRUCTIONANALIZER_H

#define INSTRUCTIONANALIZER_H

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
        ReadDirectiveFile();
        ReadInstructionFile();
    }
    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - tamanho
    */
    bool IsDirective(std::string label);

    void ReadDirectiveFile();

    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - código - tamanho
    */
    bool IsInstruction(std::string label);

    int GetInstructionOpCode(std::string label)
    {
        for (int i = 0; i < _instruction.size(); i++)
        {
            if (boost::iequals(_instruction[i].name, label))
                return _instruction[i].op_code;
        }
    }

    int InstructionOpNumber(std::string label);

    void ReadInstructionFile();

    bool IsLable(std::string lable)
    {
        if (lable.back() == ':')
            return true;

        return false;
    }

    bool IsComment(std::string lable)
    {
        if (lable.front() == ';')
            return true;

        return false;
    }

  private:
    std::vector<Directive> _directive;
    std::vector<Instruction> _instruction;
};

#endif