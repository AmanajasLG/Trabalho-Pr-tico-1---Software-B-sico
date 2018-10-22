#include "../include/junta_arquivos.hpp"

// Funcao verifica se todos os arquivos incluidos pelo usuario podem ser abertos
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

    // Se puder ela ja gera  o arquivo .e
    file_e.open(files_name[0] + ".e", std::ofstream::out);
    return true;
}

// Funcao para juntar os arquivos
bool JoinFiles::Join()
{
    int positionCounter = 0;
    int cor_factor;
    std::string line = "";
    std::vector<std::string> words;
    std::ifstream file;
    bool waitCode = false;

    bool firstLine = true;

    //Para cada arquivo:
    for (int i = 0; i < files_name.size(); i++)
    {
        cor_factor = positionCounter;
        //Gera uma tabela de uso e uma tabela de definicoes geral e tambem uma tabela de enderecos que sao relativos
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
                // Verifica se o arquivo e modulo
                if (!IsNumber(words[0]))
                {
                    // Se for ativa a flag que espera chegar na sessao code para seguir
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

            /* Soma o tamanho do vetor com o contador de posicoes ja que todos os elementos sao validos para a contagem */
            positionCounter += words.size();

            int j = 0, i = memory.GetMemorySize();

            while (i < positionCounter)
            {
                /*Se o endereco estiver na tabela de uso */
                if (tables.IsAddrInUseTable(i) != -1)
                {
                    /* Ele varifica se ele ja esta definido na tabela de definicoes */
                    if (!tables.IsSymbolInDefinitionTable(tables.GetUseSymbolName(tables.IsAddrInUseTable(i))))
                    {
                        /*  Caso nao ele coloca  o mesmo valor do arquivo e salva o endereco da memoria que precisa ser madificado depois  */
                        memory.IncludeMemorySpace(std::stoi(words[j]));
                        varNotDefined.push_back(i);
                    }
                    else
                    {
                        /* Se estiver ele soma o valor contido na tabela com o contido na posicao em questao*/
                        memory.IncludeMemorySpace(tables.GetDefinitionAddr(tables.GetUseSymbolName(tables.IsAddrInUseTable(i))) + std::stoi(words[j]));
                    }
                    i++;
                }
                /*Se o endereco for relativo ele salva seu valor + o fator de correcao */
                else if (tables.IsAddrInRelative(i))
                {
                    memory.IncludeMemorySpace(std::stoi(words[j]) + cor_factor);
                    i++;
                }
                /* Caso contrario ele apenas salva o valor normal */
                else
                {
                    memory.IncludeMemorySpace(std::stoi(words[j]));
                    i++;
                }

                j++;
            }
        }
        firstLine = true;
        file.close();
    }

    /*Varifica se ha algum elemento que nao foi definido e muda o valor da posicao*/
    for (int i = 0; i < varNotDefined.size(); i++)
    {
        memory.ChangeMemorySpaceValue(varNotDefined[i], tables.GetDefinitionAddr(tables.GetUseSymbolName(tables.IsAddrInUseTable(varNotDefined[i]))) + memory.GetMemoryPositionValue(varNotDefined[i]));
    }

    WriteEFile();
}

/*Funcao para construir as tabelas*/
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
            /* Verifica se o arquivo aberto nao é um modulo e se assim for ele sai do 
            arquivo ja que nao ha tabelas para avaliar */
            if (IsNumber(words[0]))
            {
                break;
            }

            firstLine = false;
        }

        /* Chegando na sessao CODE ele sai */
        if (words[0] == "CODE")
        {
            break;
        }
        /* Para salvar em cada tabela sao usadas as variaveis de Table para indicar em qual sessao o leitor esta */
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

    file.close();
}

/* Funcao que escreve o arquivo .e */
void JoinFiles::WriteEFile()
{
    for (int i = 0; i < memory.GetMemorySize(); i++)
    {
        file_e << memory.GetMemoryPositionValue(i) << " ";
    }
}