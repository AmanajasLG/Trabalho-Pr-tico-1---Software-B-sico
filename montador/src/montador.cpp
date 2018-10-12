#include <stdio.h>
#include <stdbool.h>
#include "/home/amanajas/Desktop/TrabalhoPratico1-SoftwareBasico/montador/include/intrucoes.hpp"
#include "/home/amanajas/Desktop/TrabalhoPratico1-SoftwareBasico/montador/include/diretivas.hpp"

int main()
{
    std::cout << "UE FDP" << std::endl;
    Instructions *inst = new Instructions();
    inst->readInstructionFile();

    if (inst->isInstruction("add"))
    {
        std::cout << "AE CARALHO" << std::endl;
    }

    Directives *dire = new Directives();
    dire->readDirectiveFile();

    return 0;
}
