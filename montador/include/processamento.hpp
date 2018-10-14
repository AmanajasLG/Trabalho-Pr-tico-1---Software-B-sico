#include "../include/analisador_de_instrucao.hpp"
#include "../include/tabelas.hpp"

/*
*   Processamento 
*   Gera arquivo .obj
*/
class PreProcessing
{
  public:
    PreProcessing(std::string file_name, Tables tables, InstructionAnalizer analizer) : file_name(file_name), tables(tables), analizer(analizer)
    {
        file_pre.open(file_name + ".pre");
    }

    ~PreProcessing()
    {
        file_pre.close();
        file_obj.close();
    }

    void FirstPass();
    void SecondPass();

    bool isFileOpen();

  private:
    std::string file_name;
    std::ifstream file_pre;
    std::ofstream file_obj;
    Tables tables;
    InstructionAnalizer analizer;
};