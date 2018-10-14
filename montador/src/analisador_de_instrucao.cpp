#include "../include/analisador_de_instrucao.hpp"
#include <boost/algorithm/string.hpp>

void InstructionAnalizer::readDirectiveFile()
{
    std::string line;
    std::ifstream file;
    file.open("diretivas.txt");
    if (!file.is_open())
    {
        std::cout << "Error - File not open" << std::endl;
        return;
    }

    while (!file.eof())
    {
        getline(file, line);
        std::istringstream iss(line);
        std::vector<std::string> result;
        for (std::string s; iss >> s;)
            result.push_back(s);

        _directive.push_back(Directive{result[0], std::stoi(result[1]), std::stoi(result[2])});
    }
}

bool InstructionAnalizer::isDirective(std::string label)
{
    for (int i = 0; i < _directive.size(); i++)
    {
        if (boost::iequals(label, _directive[i].name))
            return true;
    }
    return false;
}

void InstructionAnalizer::readInstructionFile()
{
    std::string line;
    std::ifstream file;
    file.open("instrucoes.txt");
    if (!file.is_open())
    {
        std::cout << "Error - File not open" << std::endl;
        return;
    }

    while (!file.eof())
    {
        getline(file, line);
        std::istringstream iss(line);
        std::vector<std::string> result;
        for (std::string s; iss >> s;)
            result.push_back(s);

        _instruction.push_back(Instruction{result[0], std::stoi(result[1]), std::stoi(result[2]), std::stoi(result[3])});
    }
}

bool InstructionAnalizer::isInstruction(std::string label)
{
    for (int i = 0; i < _instruction.size(); i++)
    {
        if (boost::iequals(label, _instruction[i].name))
            return true;
    }
    return false;
}