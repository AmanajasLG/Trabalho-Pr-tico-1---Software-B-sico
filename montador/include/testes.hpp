#include "../include/analisador_de_instrucao.hpp"
#include "../include/tabelas.hpp"

class Tests
{
public:
  Tests(InstructionAnalizer analizer, Tables *tables) : analizer(analizer), tables(tables) {}

  void TestIsModule(std::string firstLine);

  bool MissingDeclarationOrLable(std::string lable);
  bool RepitedDeclarationOrLable(std::string lable);
  bool JumpToInvalidLable(std::string lable);
  bool JumpToWrongSection(std::string lable);
  bool IsInvalidDirective(std::string lable);
  bool IsInvalidInstruction(std::string lable);
  bool DirectiveOrInstructionInWrongSection(std::string lable);
  bool IsDivisionByZero(std::string dividend);
  bool HasIncorrectOpNumber(std::string line);
  bool IsInvalidToken(std::string token);
  bool TwoLablesInLine(std::string line);
  bool SectionTextMissing(std::ifstream file);
  bool IsInvalidSection(std::string line);
  bool IsInvalidArgumentType(std::string arg);
  bool IsChangingConstValue(std::string arg);

  bool IsModule() { return isModule; }
  void SetIsInTextSection()
  {
    isInTextSection = true;
    isInBssSection = false;
    isInDataSection = false;
  }
  bool IsInTextSection() { return isInTextSection; }
  void SetIsInBssSection()
  {
    isInTextSection = false;
    isInBssSection = true;
    isInDataSection = false;
  }
  bool IsInBssSection() { return isInBssSection; }
  void SetIsInDataSection()
  {
    isInTextSection = false;
    isInBssSection = false;
    isInDataSection = true;
  }
  bool IsInDataSection() { return isInDataSection; }

private:
  InstructionAnalizer analizer;
  Tables *tables;
  bool isModule = false;
  bool isInTextSection = false;
  bool isInDataSection = false;
  bool isInBssSection = false;
};