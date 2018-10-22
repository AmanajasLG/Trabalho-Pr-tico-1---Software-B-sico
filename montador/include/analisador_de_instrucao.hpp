#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <boost/algorithm/string.hpp>

#ifndef INSTRUCTIONANALIZER_H
/*  
*   AnalisadorDeIstrucoes
*   Classe para identificar, analisar e retornar informacoes de 
*   instrucoes, diretivas, rotulos e comentarios. 
*/
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

    /* Retorna o tamanho da diretiva */
    int GetDirectiveSize(std::string label)
    {
        for (int i = 0; i < _directive.size(); i++)
        {
            if (boost::iequals(_directive[i].name, label))
                return _directive[i].size;
        }
    }

    /* Retorna o numero de operandos da diretiva */
    int GetDirectiveOpNumber(std::string label)
    {
        for (int i = 0; i < _directive.size(); i++)
        {
            if (boost::iequals(_directive[i].name, label))
                return _directive[i].op_number;
        }
    }

    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - código - tamanho
    */
    bool IsInstruction(std::string label);

    /* Retorna o codigo da instrucao */
    int GetInstructionOpCode(std::string label)
    {
        for (int i = 0; i < _instruction.size(); i++)
        {
            if (boost::iequals(_instruction[i].name, label))
                return _instruction[i].op_code;
        }
    }

    /* Retorna o numero de operandos da instrucao */
    int GetInstructionOpNumber(std::string label)
    {
        for (int i = 0; i < _instruction.size(); i++)
        {
            if (boost::iequals(_instruction[i].name, label))
                return _instruction[i].op_number;
        }
    }

    /* Retorna o tamanho da instrucao */
    int GetInstructionSize(std::string label)
    {
        for (int i = 0; i < _instruction.size(); i++)
        {
            if (boost::iequals(_instruction[i].name, label))
                return _instruction[i].size;
        }
    }

    void ReadInstructionFile();

    /* Verifica de o token passado eh um rotulo */
    bool IsLable(std::string lable)
    {
        if (lable.back() == ':')
            return true;

        return false;
    }

    /* Verifica de o token passado eh um comentario */
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