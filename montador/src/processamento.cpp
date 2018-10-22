#include "../include/processamento.hpp"
#include <boost/algorithm/string.hpp>
#include <limits>
#include <sstream>

/* Funcao verifica se o arquivo incluido pelo usuario pode ser aberto */
bool Processing::isFileOpen()
{
    if (!file_pre.is_open())
    {
        std::cout << "Erro - Arquivo .pre nao pode ser aberto" << std::endl;
        return false;
    }

    /* Se puder ela ja gera  o arquivo .obj */
    file_obj.open("../ligador/" + file_name + ".obj", std::ofstream::out);
    return true;
}

/* Funcao responsavel pela primeira passagem do montador */
bool Processing::FirstPass()
{
    int lineCounter = 1;
    int positionCounter = 0;
    std::string line = "";
    std::string write_line = "";
    std::vector<std::string> words;

    while (!file_pre.eof())
    {
        std::getline(file_pre, line);
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n' || c == '\0'; });

        for (int i = 0; i < words.size(); i++)
        {
            if (words[i] == "")
            {
                words.erase(words.begin() + i);
            }
        }

        if (words.size() == 0)
            continue;

        if (boost::iequals(words[0], "SECTION") && tests.DefineSection(words[1]))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

        /* Verifica erros na primeira passagem */
        if (tests.ErrorFirstPass(words))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

        /* Verifica se o token eh um rotulo para ser adicionada no tabela de simbolos */
        if (analizer.IsLable(words[0]))
        {
            /* 
                Ja adiciona informacoes na tabela para saber se o rotulo eh:
                - um variavel
                - constante
                - igual a zero
                - vetor
                - externo
            */
            if (words.size() == 1)
            {
                tables->AddElementSymbolTable(words[0], positionCounter, tests.GetSection());
            }
            else if (boost::iequals(words[1], "EXTERN"))
            {
                tables->AddElementSymbolTable(words[0], positionCounter, tests.GetSection(), false, 0, false, false, true);
            }
            else if (boost::iequals(words[1], "CONST"))
            {
                tables->AddElementSymbolTable(words[0], positionCounter, tests.GetSection(), true, std::stoi(words[2]), std::stoi(words[2]) == 0, true);
            }
            else if (boost::iequals(words[1], "SPACE"))
            {
                if (words.size() == 2)
                    tables->AddElementSymbolTable(words[0], positionCounter, tests.GetSection(), true);
                else
                    tables->AddElementSymbolTable(words[0], positionCounter, tests.GetSection(), true, 0, false, false, false, true, std::stoi(words[2]));
            }
            else
            {
                tables->AddElementSymbolTable(words[0], positionCounter, tests.GetSection());
            }
        }

        /* Contador de posicao baseado no tamanho de cada instrucao */
        for (int i = 0; i < words.size(); i++)
        {
            if (analizer.IsInstruction(words[i]))
            {
                positionCounter += analizer.GetInstructionSize(words[i]);
                break;
            }
            else if (analizer.IsDirective(words[i]))
            {

                if (boost::iequals(words[i], "SPACE") && words.size() > (i + 1))
                {
                    positionCounter += std::stoi(words[i + 1]);
                }
                else
                {
                    positionCounter += analizer.GetDirectiveSize(words[i]);
                }

                break;
            }
        }

        lineCounter++;
    }

    /* Reseta as informacoes da sessao para nao dar erro na segunda passagem */
    tests.ResetSection();

    return true;
}

/* Funcao responsavel pela segunda passagem do montador */
bool Processing::SecondPass()
{
    /* Volta para o inicio do arquivo */
    file_pre.clear();
    file_pre.seekg(0, std::ios::beg);

    int lineCounter = 1;
    int positionCounter = 0;
    std::string line = "";
    std::vector<std::string> words;

    bool firstLine = true;

    while (!file_pre.eof())
    {
        std::getline(file_pre, line);
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n'; });

        /* Testa se o arquivo eh um modulo */
        if (firstLine)
        {
            tests.TestIsModule(line);
            firstLine = false;
        }

        for (int i = 0; i < words.size(); i++)
        {
            if (words[i] == "")
            {
                words.erase(words.begin() + i);
            }
        }

        if (words.size() == 0)
            continue;

        /* Se o token for SECTION testa se eh valida */
        if (boost::iequals(words[0], "SECTION") && tests.DefineSection(words[1]))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

        /* Verifica erros na primeira passagem */
        if (tests.ErrorSecondPass(words))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

        /* Adiciona tokens externos a tabela de uso e verifica
        se o endereco eh relativo, se for adiciona na lista de relativos*/
        for (int i = 0; i < words.size(); i++)
        {
            if (tables->IsSymbolInSymbolTable(words[i]) && tables->IsSymbolExtern(words[i]))
            {
                tables->AddElementUseTable(words[i], positionCounter + i);
            }
            else if (analizer.IsInstruction(words[i]) && analizer.GetInstructionSize(words[i]) != 1)
            {
                int j;

                if (i == 0)
                    j = 1;
                else
                    j = i;

                if (analizer.IsLable(words[0]) && !boost::iequals(words[1], "COPY"))
                {
                    j++;

                    while (words.size() > j)
                    {
                        relative.push_back(positionCounter + 1);
                        j++;
                    }
                }
                else
                {
                    while (words.size() > j)
                    {
                        relative.push_back(positionCounter + j);
                        j++;
                    }
                }

                while (words.size() > j)
                {
                    relative.push_back(positionCounter + j);
                    j++;
                }
            }
        }

        /* Adiciona token publicos a tabelas de definicoes */
        if (boost::iequals(words[0], "PUBLIC") && tables->IsSymbolInSymbolTable(words[1]))
        {

            if (tables->IsSymbolConst(words[1]))
            {
                tables->AddElementDefinitionTable(words[1], tables->GetSymbolValue(words[1]));
            }
            else
            {
                tables->AddElementDefinitionTable(words[1], tables->GetSymbolAddr(words[1]));
            }
        }

        /* Contador de posicao baseado no tamanho de cada instrucao */
        for (int i = 0; i < words.size(); i++)
        {

            if (analizer.IsInstruction(words[i]))
            {
                positionCounter += analizer.GetInstructionSize(words[i]);
            }
            else if (analizer.IsDirective(words[i]))
            {

                if (boost::iequals(words[i], "SPACE") && words.size() > (i + 1))
                {
                    positionCounter += std::stoi(words[i + 1]);
                }
                else
                {
                    positionCounter += analizer.GetDirectiveSize(words[i]);
                }
            }
        }

        int j = 0, i = memory->GetMemorySize(), loopsCount = 0, dif = 0;

        /* Adiciona enderecos em cada posicao de memoria */
        while (i < positionCounter)
        {

            if (analizer.IsInstruction(words[j]))
            {
                memory->IncludeMemorySpace(analizer.GetInstructionOpCode(words[j]));
                i++;
                for (int k = 1; k <= analizer.GetInstructionOpNumber(words[j]); k++)
                {
                    if (tables->IsSymbolInSymbolTable(words[j + k + dif]))
                    {
                        /* Verificacao de argumentos com '+', testa se o rotulo eh vetor*/
                        if (tables->IsSymbolVector(words[j + k + dif]) && words.size() > 2 && words[j + k + 1 + dif] == "+")
                        {
                            memory->IncludeMemorySpace(tables->GetSymbolAddr(words[j + k + dif]) + std::stoi(words[j + k + dif + 2]));
                            dif += 2;
                            i++;
                        }
                        else
                        {
                            memory->IncludeMemorySpace(tables->GetSymbolAddr(words[j + k + dif]));
                            i++;
                        }
                    }
                    else
                    { /* Se o simbolo nao for conhecido coloca -1 */
                        memory->IncludeMemorySpace(-1);
                        i++;
                    }
                }
            }
            else if (analizer.IsDirective(words[j]))
            {
                if (boost::iequals(words[j], "CONST"))
                {
                    memory->IncludeMemorySpace(std::stoi(words[j + 1]));
                    i++;
                }
                else if (boost::iequals(words[j], "SPACE"))
                {
                    /* Faz verificacao se o simbolo eh um vetor e se for adiciona 0 em cada espaco correspondente */
                    if (words.size() > (j + 1))
                    {
                        for (int k = 0; k < std::stoi(words[j + 1]); k++)
                        {
                            memory->IncludeMemorySpace(0);
                            i++;
                        }
                    }
                    else
                    {
                        memory->IncludeMemorySpace(0);
                        i++;
                    }
                }
                else
                {
                    i++;
                }
            }
            else
            {
                loopsCount++;
            }

            if (loopsCount == 3)
                break;

            if (words.size() > (j + 1))
                j += dif + 1;
            dif = 0;
        }

        lineCounter++;
    }

    WriteObjFile();

    return true;
}

/* Funcao para escrever o arquivo .obj verificando se eh modulo ou nao */
void Processing::WriteObjFile()
{
    if (tests.IsModule())
    {
        file_obj << "TABLE USE\n";
        for (int i = 0; i < tables->GetUseTableSize(); i++)
        {
            file_obj << tables->GetUseSymbolName(i) << " " << tables->GetUseAddr(i) << "\n";
        }

        file_obj << "\nTABLE DEFINITION\n";

        for (int i = 0; i < tables->GetDefinitionTableSize(); i++)
        {
            file_obj << tables->GetDefinitionSymbolName(i) << " " << tables->GetDefinitionAddr(tables->GetDefinitionSymbolName(i)) << "\n";
        }

        file_obj << "\nRELATIVE\n";

        for (int i = 0; i < relative.size(); i++)
        {
            file_obj << relative[i] << " ";
        }

        file_obj << "\n\nCODE\n";
    }

    for (int i = 0; i < memory->GetMemorySize(); i++)
    {
        file_obj << memory->GetMemoryPositionValue(i) << " ";
    }
}