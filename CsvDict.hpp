#include <map>

class CsvDict
{
    public:
    CsvDict();
    ~CsvDict();
    void loadDictFromCsvFile();
    void printDict();
    std::string search(const std::string _key);
    std::string searchAprox(const std::string _key);

    private:
    static std::map<std::string,std::string> dict;
    void appendDictMap(const std::string&);
};


