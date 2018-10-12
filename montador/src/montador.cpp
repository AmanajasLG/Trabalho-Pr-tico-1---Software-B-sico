#include <stdio.h>
#include <stdbool.h>
#include "/home/amanajas/Desktop/TrabalhoPratico1-SoftwareBasico/montador/include/intrucoes.hpp"
#include "/home/amanajas/Desktop/TrabalhoPratico1-SoftwareBasico/montador/include/diretivas.hpp"

int main()
{
    Instructions *inst = new Instructions();
    inst->readInstructionFile();

    Directives *dire = new Directives();
    dire->readDirectiveFile();

    return 0;
}
