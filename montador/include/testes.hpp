#include "../include/analisador_de_instrucao.hpp"
#include "../include/tabelas.hpp"

#ifndef TESTS_H
/*  
*   Testes
*   Classe responsavel por verificar se o codigo eh valido e pode ser compilado
*/
#define TESTS_H

class Tests
{
public:
  Tests(InstructionAnalizer analizer, Tables *tables) : analizer(analizer), tables(tables) {}

  /* Testes de verificacao de codigo */
  void TestIsModule(std::string firstLine);

  bool MissingDeclarationOrLable(std::vector<std::string> words);
  bool RepitedDeclarationOrLable(std::string lable);
  bool JumpToInvalidLable(std::string lable);
  bool JumpToWrongSection(std::string lable);
  bool IsInvalidDirective(std::string lable);
  bool IsInvalidInstruction(std::string lable);
  bool DirectiveOrInstructionInWrongSection(std::string lable);
  bool IsDivisionByZero(std::string dividend);
  bool IsInvalidToken(std::string token);
  bool TwoLablesInLine(std::vector<std::string> words);
  bool IsInvalidArgumentType(std::vector<std::string> words);
  bool IsChangingConstValue(std::string arg);
  bool HasWrongOpNumber(std::vector<std::string> words);
  bool IsInVectorRange();

  /* Funcoes destinadas a identificacao da sessao atual */
  bool DefineSection(std::string section);
  bool IsModule() { return isModule; }
  std::string GetSection()
  {
    if (isInTextSection)
    {
      return "TEXT";
    }
    else if (isInBssSection)
    {
      return "BSS";
    }
    else if (isInDataSection)
    {
      return "DATA";
    }
    else
    {
      return "";
    }
  }
  bool IsInTextSection() { return isInTextSection; }
  bool IsInBssSection() { return isInBssSection; }
  bool IsInDataSection() { return isInDataSection; }
  void ResetSection()
  {
    isInTextSection = false;
    passSectionText = false;
    isInDataSection = false;
    isInBssSection = false;
  }

  bool IsNumber(std::string word) { return word.find_first_not_of("-0123456789,") == std::string::npos; }

  /* Funcoes para executar testes especificos em cada uma das passagens */
  bool ErrorFirstPass(std::vector<std::string> words);
  bool ErrorSecondPass(std::vector<std::string> words);

private:
  InstructionAnalizer analizer;
  Tables *tables;
  bool isModule = false;
  bool isInTextSection = false;
  bool passSectionText = false;
  bool isInDataSection = false;
  bool isInBssSection = false;
};

#endif