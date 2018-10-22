#include "../include/testes.hpp"
#include <boost/algorithm/string.hpp>
#include <ctype.h>

void Tests::TestIsModule(std::string firstLine)
{
    std::vector<std::string> words;
    boost::split(words, firstLine, [](char c) { return c == ' ' || c == '\n' || c == '\0'; });

    if (analizer.IsLable(words[0]) && analizer.IsDirective(words[1]) && boost::iequals(words[1], "BEGIN"))
        isModule = true;
}

bool Tests::MissingDeclarationOrLable(std::vector<std::string> words)
{

    if (analizer.IsLable(words[0]))
        words.erase(words.begin());

    if (words.size() == 1)
        return false;

    for (int i = 0; i < words.size(); i++)
    {
        if (analizer.IsInstruction(words[i]))
        {
            if (tables->IsSymbolInSymbolTable(words[i + 1]))
            {
                return words.size() < analizer.GetInstructionSize(words[i]);
            }
        }
        else if (analizer.IsDirective(words[i]))
        {
            return (words.size() - i) < analizer.GetDirectiveOpNumber(words[i]);
        }
        else
        {
            return !IsNumber(words[i]) && !tables->IsSymbolInSymbolTable(words[i]);
        }
    }
    return false;
}

bool Tests::RepitedDeclarationOrLable(std::string lable)
{
    return tables->IsSymbolInSymbolTable(lable);
}

bool Tests::JumpToInvalidLable(std::string lable)
{
    return !tables->IsSymbolInSymbolTable(lable);
}

bool Tests::JumpToWrongSection(std::string lable)
{
    return !boost::iequals(tables->SymbolSection(lable), "TEXT");
}

bool Tests::IsInvalidDirective(std::string lable)
{
    return !analizer.IsDirective(lable);
}

bool Tests::IsInvalidInstruction(std::string lable)
{
    return !analizer.IsInstruction(lable);
}

bool Tests::DirectiveOrInstructionInWrongSection(std::string lable)
{
    if (isInBssSection)
    {
        if (boost::iequals(lable, "SECTION") || boost::iequals(lable, "END"))
            return false;

        return analizer.IsInstruction(lable) || (analizer.IsDirective(lable) && !boost::iequals(lable, "SPACE"));
    }
    else if (isInDataSection)
    {
        if (boost::iequals(lable, "SECTION") || boost::iequals(lable, "END"))
            return false;

        return analizer.IsInstruction(lable) || (analizer.IsDirective(lable) && !boost::iequals(lable, "CONST"));
    }
    else if (isInTextSection)
    {
        if (boost::iequals(lable, "SECTION") || boost::iequals(lable, "END"))
            return false;

        return !analizer.IsInstruction(lable) && (analizer.IsDirective(lable) && (boost::iequals(lable, "CONST") || boost::iequals(lable, "SPACE")));
    }
    else
    {
        return !analizer.IsInstruction(lable) && (analizer.IsDirective(lable) && !boost::iequals(lable, "BEGIN"));
    }
}

bool Tests::IsDivisionByZero(std::string dividend)
{
    return boost::iequals(dividend, "0") || (tables->IsSymbolInSymbolTable(dividend) && tables->IsSymbolValueZero(dividend));
}

bool Tests::IsInvalidToken(std::string token)
{
    if (IsNumber(token) && !isInTextSection)
        return false;

    if (token.length() > 50 || isdigit(token[0]))
        return true;

    if (token.length() == 1 && token == "+")
    {
        return false;
    }

    if (token.back() == ':' || token.back() == ',')
        token.pop_back();

    for (int i = 0; i < token.length(); i++)
    {
        if (!isalnum(token[i]) && token[i] != '_')
        {
            return true;
        }
    }

    return false;
}

bool Tests::TwoLablesInLine(std::vector<std::string> words)
{
    bool hasLabel = false;

    for (int i = 0; i < words.size(); i++)
    {
        if (analizer.IsLable(words[i]) && !hasLabel)
        {
            hasLabel = true;
        }
        else if (analizer.IsLable(words[i]) && hasLabel)
        {
            std::cout << "2 LABLES NA LINHA - ERRO SINTATICO" << std::endl;
            return true;
        }
    }
    return false;
}

bool Tests::IsInvalidArgumentType(std::vector<std::string> words)
{

    for (int i = 0; i < words.size(); i++)
    {
        if (analizer.IsInstruction(words[i]))
        {
            if (boost::iequals(words[i], "ADD") || boost::iequals(words[i], "SUB") || boost::iequals(words[i], "MULT") || boost::iequals(words[i], "DIV"))
            {
                if (tables->IsSymbolVector(words[i + 1]) && words.size() > 2)
                {
                    if (!IsNumber(words[i + 3]))
                        return true;

                    return !(words.size() == 2) && (!(words[i + 2] == "+") || std::stoi(words[i + 3]) < 0) || !(std::stoi(words[i + 3]) < tables->GetVectorSize(words[i + 1]));
                }
                return !tables->IsSymbolExtern(words[i + 1]) && !tables->IsSymbolVariable(words[i + 1]) && !IsNumber(words[i + 1]);
            }
            else if (boost::iequals(words[i], "COPY"))
            {
                if (tables->IsSymbolVector(words[i + 1]) && words[i + 2] == "+")
                {
                    return (!tables->IsSymbolVariable(words[i + 4]) && !IsNumber(words[i + 4]));
                }
                return !tables->IsSymbolVariable(words[i + 1]) || (!tables->IsSymbolVariable(words[i + 2]) && !IsNumber(words[i + 2]));
            }
            else if (boost::iequals(words[i], "STORE") || boost::iequals(words[i], "INPUT"))
            {
                if (tables->IsSymbolVector(words[i + 1]) && words.size() > 2)
                {
                    if (!IsNumber(words[i + 3]))
                        return true;

                    return !(words.size() == 2) && (!(words[i + 2] == "+") || std::stoi(words[i + 3]) < 0) || !(std::stoi(words[i + 3]) < tables->GetVectorSize(words[i + 1]));
                }
                return !tables->IsSymbolExtern(words[i + 1]) && !tables->IsSymbolVariable(words[i + 1]);
            }
            else if (boost::iequals(words[i], "JMP") || boost::iequals(words[i], "JMPN") || boost::iequals(words[i], "JMPP") || boost::iequals(words[i], "JMPZ"))
            {
                return tables->IsSymbolVariable(words[i + 1]) || IsNumber(words[i + 1]);
            }
        }
        else if (analizer.IsDirective(words[i]))
        {
            if (boost::iequals(words[i], "SPACE"))
            {
                std::cout << "ENTRO AQUI" << std::endl;
                return !(words.size() == 2) && (!IsNumber(words[i + 1]) || std::stoi(words[i + 1]) < 0);
            }
            else if (boost::iequals(words[i], "CONST"))
            {
                return !IsNumber(words[i + 1]);
            }
        }
    }
}

bool Tests::IsChangingConstValue(std::string arg)
{
    return tables->IsSymbolConst(arg);
}

bool Tests::DefineSection(std::string section)
{
    if (boost::iequals(section, "TEXT"))
    {
        isInTextSection = true;
        passSectionText = true;
        isInBssSection = false;
        isInDataSection = false;
        return false;
    }
    else if (boost::iequals(section, "BSS"))
    {
        if (!passSectionText)
        {
            std::cout << "Erro Semantico - Sem Sessao TEXT!" << std::endl;
            return true;
        }
        isInTextSection = false;
        isInBssSection = true;
        isInDataSection = false;
        return false;
    }
    else if (boost::iequals(section, "DATA"))
    {
        if (!passSectionText)
        {
            std::cout << "Erro Semantico - Sem Sessao TEXT!" << std::endl;
            return true;
        }
        isInTextSection = false;
        isInBssSection = false;
        isInDataSection = true;
        return false;
    }
    else
    {
        std::cout << "Erro Sintatico - Sessao invalida!" << std::endl;
        return true;
    }
}

bool Tests::HasWrongOpNumber(std::vector<std::string> words)
{
    if (analizer.IsLable(words[0]))
        words.erase(words.begin());

    if (analizer.IsInstruction(words[0]))
    {
        if (words.size() == 1)
        {
            return (words.size() - 1) != analizer.GetInstructionOpNumber(words[0]);
        }
        if (boost::iequals(words[0], "COPY") && words.size() > 5)
        {
            if (tables->IsSymbolInSymbolTable(words[1]) && tables->IsSymbolVector(words[1]) && words[2] == "+" && tables->IsSymbolInSymbolTable(words[4]) && tables->IsSymbolVector(words[4]) && words[5] == "+")
                return (words.size() - 1) != (analizer.GetInstructionOpNumber(words[0]) + 4);
        }
        else if (tables->IsSymbolInSymbolTable(words[1]) && tables->IsSymbolVector(words[1]) && words.size() > 2 && words[2] == "+")
        {
            return (words.size() - 1) != (analizer.GetInstructionOpNumber(words[0]) + 2);
        }

        return (words.size() - 1) != analizer.GetInstructionOpNumber(words[0]);
    }

    for (int i = 0; i < words.size(); i++)
    {
        if (analizer.IsDirective(words[i]))
        {
            if (boost::iequals(words[i], "SPACE"))
                return (words.size() - 1 - i) != 1 && (words.size() - 1 - i) != 0;

            return (words.size() - 1 - i) != analizer.GetDirectiveOpNumber(words[i]);
        }
    }
}

bool Tests::ErrorFirstPass(std::vector<std::string> words)
{

    if (!analizer.IsLable(words[0]) && IsInvalidDirective(words[0]) && IsInvalidInstruction(words[0]))
    {
        std::cout << "Erro Lexico - Diretiva ou Instrução invalida!" << std::endl;
        return true;
    }

    for (int i = 0; i < words.size(); i++)
    {
        if (IsInvalidToken(words[i]))
        {
            if (i > 0 && words[i - 1] == "+")
                continue;

            std::cout << "Erro Lexico - Token invalido!" << std::endl;
            return true;
        }
        else if (analizer.IsLable(words[i]) && RepitedDeclarationOrLable(words[i]))
        {
            std::cout << "Erro Semantico - Declaracao ou Rotulo repetido!" << std::endl;
            return true;
        }
    }

    if (TwoLablesInLine(words))
    {
        std::cout << "Erro Sintatico - Dois rotulos em uma linha!" << std::endl;
        return true;
    }

    return false;
}

bool Tests::ErrorSecondPass(std::vector<std::string> words)
{
    if (words.size() == 0)
        return false;

    for (int i = 0; i < words.size(); i++)
    {

        if (analizer.IsInstruction(words[i]) || analizer.IsDirective(words[i]))
        {

            if (DirectiveOrInstructionInWrongSection(words[i]))
            {
                std::cout << "Erro Semantico - Instrucao ou Diretiva na sessao errada!" << std::endl;
                return true;
            }
            else if ((boost::iequals(words[i], "JMP") || boost::iequals(words[i], "JMPN") || boost::iequals(words[i], "JMPP") || boost::iequals(words[i], "JMPZ")))
            {

                if (JumpToInvalidLable(words[i + 1]))
                {
                    std::cout << "Erro Semantico - Pulo para rotulo invalido!" << std::endl;
                    return true;
                }
                else if (JumpToWrongSection(words[i + 1]))
                {
                    std::cout << "Erro Semantico - Pulo para sessao errada!" << std::endl;
                    return true;
                }
            }
            else if (boost::iequals(words[i], "DIV") && IsDivisionByZero(words[i + 1]))
            {
                std::cout << "Erro Sintatico - Divisao por zero!" << std::endl;
                return true;
            }
            else if ((boost::iequals(words[i], "COPY") || boost::iequals(words[i], "STORE") || boost::iequals(words[i], "INPUT")) && IsChangingConstValue(words[i + 1]))
            {
                std::cout << "Erro Semantico - Alteracao de variavel constante!" << std::endl;
                return true;
            }
        }
    }

    if (MissingDeclarationOrLable(words))
    {
        std::cout << "Erro Sintatico - Declaracao ou Rotulo ausente!" << std::endl;
        return true;
    }
    else if (IsInvalidArgumentType(words))
    {
        std::cout << "Erro Sintatico - Tipo de argumento invalido!" << std::endl;
        return true;
    }
    else if (HasWrongOpNumber(words))
    {
        std::cout << "Erro Sintatico - Numero invalido de operandos!" << std::endl;
        return true;
    }

    return false;
}