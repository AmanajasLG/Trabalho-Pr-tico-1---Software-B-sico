#include "../include/junta_arquivos.hpp"

bool JoinFiles::IsValidFile()
{
    for (int i = 0; i < files_name.size(); i++)
    {
        std::ifstream file;
        file.open(files_name[i] + ".obj");
        if (!file.is_open())
        {
            std::cout << "Erro - Arquivo " << files_name[i] << " nao pode ser aberto" << std::endl;
            std::cout << "Verifique o nome do arquivo - não deve conter extensao" << std::endl;
            return false;
        }
    }

    file_e.open(files_name[0] + ".e", std::ofstream::out);
    return true;
}

bool JoinFiles::Join()
{
    int positionCounter = 0;
    int cor_factor;
    std::string line = "";
    std::vector<std::string> words;
    std::ifstream file;
    bool waitCode = false;

    bool firstLine = true;

    for (int i = 0; i < files_name.size(); i++)
    {
        cor_factor = positionCounter;
        BuildTables(i, cor_factor);
        file.open(files_name[i] + ".obj");

        while (!file.eof())
        {
            std::getline(file, line);
            boost::split(words, line, [](char c) { return c == ' ' || c == '\n'; });

            if (waitCode)
            {
                if (words[0] == "CODE")
                    waitCode = false;
                continue;
            }

            if (firstLine)
            {
                if (!IsNumber(words[0]))
                {
                    waitCode = true;
                    continue;
                }
                firstLine = false;
            }

            for (int i = 0; i < words.size(); i++)
            {
                if (words[i] == "")
                {
                    words.erase(words.begin() + i);
                }
            }

            positionCounter += words.size();

            int j = 0, i = memory.GetMemorySize();

            while (i < positionCounter)
            {
                std::cout << "ENTRO AQUI" << std::endl;
                if (tables.IsAddrInUseTable(i) != -1)
                {
                    std::cout << "O PROBLEMA É A PORRA DO NOME DA VAR!!POS: " << tables.IsAddrInUseTable(i) << std::endl;
                    std::cout << "NOME DA VAR: " << tables.GetUseSymbolName(tables.IsAddrInUseTable(i)) << std::endl;
                    if (!tables.IsSymbolInDefinitionTable(tables.GetUseSymbolName(tables.IsAddrInUseTable(i))))
                    {
                        memory.IncludeMemorySpace(std::stoi(words[j]));
                        varNotDefined.push_back(i);
                    }
                    else
                    {
                        memory.IncludeMemorySpace(tables.GetDefinitionAddr(tables.GetUseSymbolName(tables.IsAddrInUseTable(i))) + std::stoi(words[j]));
                    }
                    i++;
                }
                else if (tables.IsAddrInRelative(i))
                {
                    std::cout << "agora aqui fdp! " << std::endl;
                    memory.IncludeMemorySpace(std::stoi(words[j]) + cor_factor);
                    i++;
                }
                else
                {
                    std::cout << "VAI TOMA NO SEU CU" << std::endl;
                    memory.IncludeMemorySpace(std::stoi(words[j]));
                    i++;
                }

                j++;
            }
        }
        std::cout << "VARIAVEL NAO DEFINIDA: " << varNotDefined.size() << std::endl;
        ;
        firstLine = true;
        file.close();
    }

    for (int i = 0; i < varNotDefined.size(); i++)
    {
        memory.ChangeMemorySpaceValue(varNotDefined[i], tables.GetDefinitionAddr(tables.GetUseSymbolName(tables.IsAddrInUseTable(varNotDefined[i]))) + memory.GetMemoryPositionValue(varNotDefined[i]));
    }

    memory.ShowMemory();

    WriteEFile();
}

void JoinFiles::BuildTables(int file_pos, int cor_factor)
{
    std::string line = "";
    std::vector<std::string> words;
    bool firstLine = true;
    std::ifstream file;
    file.open(files_name[file_pos] + ".obj");

    while (!file.eof())
    {

        std::getline(file, line);
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n' || c == '\r'; });

        for (int i = 0; i < words.size(); i++)
        {
            if (words[i] == "")
            {
                words.erase(words.begin() + i);
            }
        }

        if (words.size() == 0)
            continue;

        if (firstLine)
        {
            if (IsNumber(words[0]))
            {
                break;
            }

            firstLine = false;
        }

        if (words[0] == "CODE")
        {
            break;
        }
        else if (words[0] == "TABLE")
        {
            tables.SetTable(words[1]);
            continue;
        }
        else if (words[0] == "RELATIVE")
        {
            tables.SetTable(words[0]);
            continue;
        }

        if (tables.IsTableUse())
        {
            tables.AddElementUseTable(words[0], std::stoi(words[1]) + cor_factor);
        }
        else if (tables.IsTableDefinition())
        {
            tables.AddElementDefinitionTable(words[0], std::stoi(words[1]) + cor_factor);
        }
        else if (tables.IsRelative())
        {
            for (int i = 0; i < words.size(); i++)
            {
                tables.AddRelativeAddr(std::stoi(words[i]) + cor_factor);
            }
        }
    }

    std::cout << "TABLE RELATIVE" << std::endl;
    for (int i = 0; i < tables.GetRelativeSize(); i++)
        std::cout << tables.GetRelativeAddr(i) << std::endl;

    tables.ShowDefinitionTable();
    tables.ShowUseTable();

    file.close();
}

void JoinFiles::WriteEFile()
{
    for (int i = 0; i < memory.GetMemorySize(); i++)
    {
        file_e << memory.GetMemoryPositionValue(i) << " ";
    }
}