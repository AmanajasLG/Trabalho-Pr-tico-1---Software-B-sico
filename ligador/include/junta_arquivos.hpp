#include "../include/tabelas.hpp"
#include "../include/memoria.hpp"

#ifndef JOINFILES_H
/*
*   JuntaArquivos
*   Classe que pega os arquivos inseridos pelo usuário, 
*   junta e gera o arquivo .e
*/
#define JOINFILES_H

class JoinFiles
{
  public:
    JoinFiles(std::vector<std::string> files_name) : files_name(files_name) {}

    bool IsValidFile();

    bool Join();
    void BuildTables(int file_pos, int cor_factor);

    void WriteEFile();
    
    void CloseFiles()
    {
        file_e.close();
    }

    bool IsNumber(std::string word) { return word.find_first_not_of("-0123456789") == std::string::npos; }

  private:
    std::vector<std::string> files_name;
    std::ofstream file_e;
    Tables tables = Tables();
    Memory memory = Memory();
    std::vector<int> varNotDefined;
};

#endif