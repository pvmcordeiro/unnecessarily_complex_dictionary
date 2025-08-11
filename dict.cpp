/*
MIT License

Copyright (c) 2025 Paulo Cordeiro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
