#include "../include/analisador_de_instrucao.hpp"
#include "../include/tabelas.hpp"

class Tests
{
  public:
    Tests(InstructionAnalizer analizer, Tables *tables) : analizer(analizer), tables(tables) {}

    bool jumpToInvaliLable(std::string);
    bool isInvalidInstruction(std::string label);
    bool isInvalidDirective(std::string label);
    bool isIncorrectOpNumber(std::string line);
    bool isDivisionByZero(std::string dividend);
    bool twoLablesInLine(std::string line);

  private:
    InstructionAnalizer analizer;
    Tables *tables;
    bool isModule;
};