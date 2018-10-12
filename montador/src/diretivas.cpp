#include "/home/amanajas/Desktop/TrabalhoPratico1-SoftwareBasico/montador/include/diretivas.hpp"

void Directives::readDirectiveFile()
{
    std::string line;
    std::ifstream file;
    file.open("diretivas.txt");
    if (!file.is_open())
    {
        std::cout << "Error - File not open" << std::endl;
        return;
    }

    while (!file.eof())
    {
        getline(file, line);
        std::istringstream iss(line);
        std::vector<std::string> result;
        for (std::string s; iss >> s;)
            result.push_back(s);

        _directive.push_back(Directive{result[0], std::stoi(result[1]), std::stoi(result[2])});
    }
}