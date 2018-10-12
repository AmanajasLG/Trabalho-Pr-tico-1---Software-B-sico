#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

class Directives
{

    struct Directive
    {
        std::string name;
        int op_number;
        int size;
    };

  public:
    /*
    * Formato arquivo instrucoes.txt
    * mnemônico - operandos - tamanho
    */
    bool isDirective()
    {
        return true;
    }

    void readDirectiveFile();

  private:
    std::vector<Directive> _directive;
};
