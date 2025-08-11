#include <iostream>
#include <memory>
#include <unordered_map>

#include "worker/CsvDict.hpp"
#include "worker/DictManager.hpp"
#include "worker/PaulosCSVParser.hpp"
#include "commons/UCDLogger.hpp"

/**
 * This small program is meant to be generat the smallest
 * executable that uses the UCD classes. It is quite 
 * straightfoward. Just loads the words from the .csv, then
 * check the keys and print the results.
 */
int main ()
{
    std::cout << "--- Unncessarily Complex Dictionary --- \n type 'quit' or 'exit' to finish the program" << "\n\n";
    std::unordered_map<std::string, std::string> myDict;
    std::string dictFileName = "dict-dutch-pt.csv";
    
    PaulosCSVParser cvsParser;
    DictManager dictManager = {myDict};

    UCDLogger::getInstance()->updateOutputVerbosity(LOG_DEBUG);

    if (! cvsParser.getDictionary(dictFileName, myDict))
    {
        UCD_LOGGER(LOG_ERR, "Not possible to parse dictionary from " + dictFileName);
        return 1;
    }
  
    std::string word;
    std::string translatedWord;

    while ( true )
    {
        std::cout << "Enter a Dutch word: " ;
        std::cin >> word;
        if (!(word.compare("exit") && word.compare("quit")))
        {
            break;
        }
        if (!dictManager.searchWord(word, translatedWord))
        {
            if (!dictManager.searchAproxWord(word, translatedWord))
            {
                UCD_LOGGER(LOG_INFO, "Word '" + word + " not found");
                continue;
            }
        }
        std::cout << translatedWord << std::endl;
    }
    return 0;
}
