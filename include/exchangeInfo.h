#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h> 


namespace exchangeInfo{
class exchangeInfoClass{
private:
    std::string jsonData;
    int id;
    std::vector<int> prevID;
    std::string instrumentName ;
    std::vector<std::string> prevInstrumentName;
    std::string contractType;
    std::string tickSize;
    std::string stepSize;
    std::string filterType;
    std::string status;



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