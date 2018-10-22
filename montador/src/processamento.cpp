#include "../include/processamento.hpp"
#include <boost/algorithm/string.hpp>
#include <limits>
#include <sstream>

bool Processing::isFileOpen()
{
    if (!file_pre.is_open())
    {
        std::cout << "Erro - Arquivo .pre nao pode ser aberto" << std::endl;
        return false;
    }

    file_obj.open("../ligador/" + file_name + ".obj", std::ofstream::out);
    return true;
}

bool Processing::FirstPass()
{
    int lineCounter = 1;
    int positionCounter = 0;
    std::string line = "";
    std::string write_line = "";
    std::vector<std::string> words;

    bool firstLine = true;

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

        if (tests.ErrorFirstPass(words))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

        if (analizer.IsLable(words[0]))
        { // igual missing só q ao contrario
            if (tests.RepitedDeclarationOrLable(words[0]))
            {
                return false;
            }
            else if (words.size() == 1)
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

    tests.ResetSection();

    return true;
}

bool Processing::SecondPass()
{
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

        if (firstLine)
        {
            tests.TestIsModule(line);
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

        if (boost::iequals(words[0], "SECTION") && tests.DefineSection(words[1]))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

        if (tests.ErrorSecondPass(words))
        {
            std::cout << "Linha " << lineCounter << std::endl;
            return false;
        }

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

        while (i <= positionCounter)
        {

            if (analizer.IsInstruction(words[j]))
            {
                memory->IncludeMemorySpace(analizer.GetInstructionOpCode(words[j]));
                i++;
                for (int k = 1; k <= analizer.GetInstructionOpNumber(words[j]); k++)
                {
                    if (tables->IsSymbolInSymbolTable(words[j + k + dif]))
                    {

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
                    {
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

                    if (words.size() > (j + 1))
                    {
                        for (int k = 0; k < std::stoi(words[j + 1]); k++)
                        {
                            memory->IncludeMemorySpace(00);
                            i++;
                        }
                    }
                    else
                    {
                        memory->IncludeMemorySpace(00);
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