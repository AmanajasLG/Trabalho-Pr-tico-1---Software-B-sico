#include "../include/preprocessamento.hpp"
#include <boost/algorithm/string.hpp>

/* Funcao verifica se o arquivo incluido pelo usuario pode ser aberto */
bool PreProcessing::isFileOpen()
{
    if (!file_asm.is_open())
    {
        std::cout << "Erro - Arquivo nao pode ser aberto" << std::endl;
        std::cout << "Verifique o nome do arquivo - não deve conter extensao" << std::endl;
        return false;
    }

    /* Se puder ela ja gera  o arquivo .pre */
    file_pre.open(file_name + ".pre", std::ofstream::out);
    return true;
}

/* Funcao responsavel pelo pre-processamento */
void PreProcessing::PreProcess()
{
    std::string line;
    std::string write_line = "";
    std::vector<std::string> words;

    while (!file_asm.eof())
    {
        std::getline(file_asm, line);
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n' || c == '\0' || c == '\t' || c == '\r'; });

        /* Verifica se a linha é vazia */
        if (words[0] != "\n")
        {
            /* Verifica palavra por palavra e se for valida, ou seja, diferente de EQU, IF e comentario, AddElementSymbolTable
            eh adicionada na frase que sera colocada no arquivo pre */
            for (int i = 0; i < words.size(); i++)
            {
                /* Foi utilizada a implementacao da tabela de simbolos para salvar os valores de EQU */
                if (analizer.IsLable(words[i]) && boost::iequals(words[i + 1], "EQU"))
                {
                    std::string lable = words[i];
                    lable.pop_back();
                    tables.AddElementSymbolTable(lable, std::stoi(words[i + 2]), "", false, (std::stoi(words[i + 2]) == 0));
                    break;
                }
                else if (analizer.IsComment(words[i]))
                {
                    break;
                }
                else if (i > 0 && boost::iequals(words[i - 1], "CONST"))
                {
                    /* Ela tambem converte valores hexa e salva no arquivo .pre como decimal  */
                    if (boost::iequals(words[i].substr(0, 2), "0x"))
                    {
                        signed int val;
                        val = std::strtol(words[i].c_str(), nullptr, 16);
                        write_line += std::to_string(val);
                    }
                    else
                    {
                        write_line += words[i];
                    }
                }
                else if (boost::iequals(words[i], "IF"))
                {
                    /* Verifica se tem alguma coisa antes do IF e ja coloca na frase */
                    if (i != 0)
                    {
                        for (int j = 0; j < i; j++)
                        {
                            if (words[j].find_first_not_of(" ") != (words[j].size() - 1))
                                write_line += words[j] + " ";
                        }
                    }

                    /* Se a variavel de IF nao existe ou eh 0, ela ja pula a linha que nao sera adicionada */
                    if (!tables.IsSymbolInSymbolTable(words[i + 1]) || tables.IsSymbolValueZero(words[i + 1]))
                    {
                        std::getline(file_asm, line);
                    }
                    break;
                }
                else if (words[i].find_first_not_of(" ") != (words[i].size() - 1))
                {
                    if (!tables.IsSymbolInSymbolTable(words[i]))
                    {
                        write_line += words[i] + " ";
                    }
                    else
                    {
                        /* Se o token for um simbolo ela ja verifica se ele tem uma virgula depois para o caso de copy nao perder o sentido */
                        if (words[i].back() == ',')
                        {
                            write_line += std::to_string(tables.GetSymbolAddr(words[i])) + ", ";
                        }
                        else
                        {
                            write_line += std::to_string(tables.GetSymbolAddr(words[i])) + " ";
                        }
                    }
                }
                else if (words[i].length() == 1 && words[i] != " ")
                {
                    write_line += words[i] + " ";
                }
            }
        }

        /* Se a linha nao for vazia ela coloca no arquivo .pre */
        if (write_line != "" && write_line != " ")
        {
            file_pre << write_line << "\n";
        }
        words.clear();
        write_line = "";
    }
}