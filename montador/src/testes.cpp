#include "../include/testes.hpp"
#include <boost/algorithm/string.hpp>
#include <ctype.h>

void Tests::TestIsModule(std::string firstLine)
{
    std::vector<std::string> words;
    boost::split(words, firstLine, [](char c) { return c == ' ' || c == '\n' || c == '\0'; });

    if (analizer.isLable(words[0]) && analizer.isDirective(words[1]) && boost::iequals(words[1], "BEGIN"))
        isModule = true;
}

bool Tests::MissingDeclarationOrLable(std::string lable)
{
    return !tables->IsSymbolInSymbolTable(lable) || !tables->IsSymbolInUseTable(lable) || !tables->IsSymbolInDefinitionTable(lable);
}

bool Tests::RepitedDeclarationOrLable(std::string lable)
{
    return tables->IsSymbolInSymbolTable(lable) || tables->IsSymbolInUseTable(lable) || tables->IsSymbolInDefinitionTable(lable);
}

bool Tests::JumpToInvalidLable(std::string lable)
{
}

bool Tests::JumpToWrongSection(std::string lable)
{
    return !boost::iequals(tables->SymbolSection(lable), "TEXT");
}

bool Tests::IsInvalidDirective(std::string lable)
{
    return !analizer.isDirective(lable);
}

bool Tests::IsInvalidInstruction(std::string lable)
{
    return !analizer.isInstruction(lable);
}

bool Tests::DirectiveOrInstructionInWrongSection(std::string lable)
{
    return (isInDataSection || IsInBssSection) && (analizer.isDirective(lable) || analizer.isInstruction(lable));
}

bool Tests::IsDivisionByZero(std::string dividend)
{
    return boost::iequals(dividend, "0") || (tables->IsSymbolInSymbolTable(dividend) && tables->GetSymbolValue(dividend) == 0);
}

bool Tests::IsInvalidToken(std::string token)
{
    if (token.length() > 50 || isdigit(token[0]))
        return true;

    for (int i = 0; i < token.length(); i++)
    {
        if (!isalnum(token[i]) && token[i] != '_')
            return true;
    }

    return false;
}