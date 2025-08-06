#include <iostream>
#include <memory>
#include "CsvDict.hpp"

int main ()
{
    std::cout << "--- Unncessarily Complex Dictionary --- " << "\n\n";

    /**
     * Opening .csv file and placing it in a map
     */
    auto a = std::shared_ptr<CsvDict>();
    a->loadDictFromCsvFile(); 
    // a->printDict();
    
    std::string word;

    while ( true )
    {
        std::cout << "Entre palavra: " ;
        std::cin >> word;
        if (!(word.compare("exit") && word.compare("quit")))
        {
            break;
        }
        std::string result = a->search(word);
        if (result.empty())
        {
            result = a->searchAprox(word);
        }
        if (!result.empty())
            std::cout << result << std::endl;
    }
    return 0;
}
