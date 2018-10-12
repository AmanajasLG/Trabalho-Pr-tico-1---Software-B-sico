#include "../include/preprocessamento.hpp"
#include <boost/algorithm/string.hpp>

bool PreProcessing::isFileOpen()
{
    if (!file_asm.is_open())
    {
        std::cout << "Erro - Arquivo nao pode ser aberto" << std::endl;
        std::cout << "Verifique o nome do arquivo - não deve conter extensao" << std::endl;
        return false;
    }

    file_pre.open(file_name + ".pre", std::ofstream::out);
    return true;
}

void PreProcessing::PreProcess()
{
    std::string line;
    std::string write_line = "";
    std::vector<std::string> words;

    while (!file_asm.eof())
    {
        std::getline(file_asm, line);
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n' || c == '\0'; });

        if (analizer.isLable(words[0]) && boost::iequals(words[1], "EQU"))
        {
            std::string lable = words[0];
            lable.pop_back();
            tables.addElementDefinitionTable(lable, std::stoi(words[2]));
        }
        else if (words[0] != "\n")
        {
            for (int i = 0; i < words.size(); i++)
            {
                if (analizer.isComment(words[i]))
                {
                    break;
                }
                else if (boost::iequals(words[i], "IF"))
                {
                    if (i != 0)
                    {
                        for (int j = 0; j < i; j++)
                        {
                            write_line += words[j] + " ";
                        }
                    }
                    if (!tables.isSymbolInDefinitionTable(words[i + 1]) && tables.getDefinitionVal(words[i + 1]) == 0)
                    {
                        std::getline(file_asm, line);
                        break;
                    }
                }
                else if (words[i].find_first_not_of(" ") != words[i].size() - 1)
                {
                    if (!tables.isSymbolInDefinitionTable(words[i]))
                    {
                        write_line += words[i] + " ";
                    }
                    else
                    {
                        write_line += std::to_string(tables.getDefinitionVal(words[i])) + " ";
                    }
                }
            }
        }

        if (write_line != "")
        {
            file_pre << write_line << "\n";
            file_pre << std::flush;
        }
        words.clear();
        write_line = "";
    }
}