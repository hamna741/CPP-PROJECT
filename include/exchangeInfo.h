#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

namespace exchangeInfo{
class exchangeInfoClass{
private:
    std::string jsonData;
    int id;
    std::string instrumentName ;
    std::string queryType;

public:
    exchangeInfoClass();
    int readQueryFile();
    void getExchangeInfo();
    void deleteData();
    void getData(std::string instrumentName);
    void updatetData();
    ~exchangeInfoClass();


};
}