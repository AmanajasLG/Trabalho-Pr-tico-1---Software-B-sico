#include "../include/testes.hpp"
#include "../include/memoria.hpp"

#ifndef PROCESSING_H

/*
*   Processamento 
*   Analisa erros e gera o arquivo .obj
*/

#define PROCESSING_H

class Processing
{
  public:
    Processing(std::string file_name, Tables *tables, InstructionAnalizer analizer) : file_name(file_name), tables(tables), analizer(analizer)
    {
        file_pre.open(file_name + ".pre");
    }

    void CloseFiles()
    {
        file_pre.close();
        file_obj.close();
    }

    bool FirstPass();
    bool SecondPass();

    void WriteObjFile();

    bool isFileOpen();

  private:
    std::string file_name;
    std::ifstream file_pre;
    std::ofstream file_obj;
    Tables *tables;
    InstructionAnalizer analizer;
    Memory *memory = new Memory();
    Tests tests = Tests(analizer, tables);
    std::vector<int> relative;
};

#endif