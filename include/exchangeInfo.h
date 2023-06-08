
#ifndef EXCHANGE_INFO_H
#define EXCHANGE_INFO_H

#include <vector>
#include <iostream>
#include <string>
#include <cpprest/http_client.h>
// #include <cpprest/filestream.h>
#include<fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <thread>
#include <chrono>
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/basic_file_sink.h"
#include <boost/asio.hpp>
// #include <boost/asio/ssl.hpp>

using boost::asio::ip::tcp;
class ExchangeInfoClass
{
private:
   
    struct SymbolData
    {
        std::string symbolName;
        std::string status;
        std::string contractType;
        std::string tickSize;
        std::string stepSize;
    };
   
   SymbolData dataObject;
    std::vector<SymbolData> symbolDataVec;
    std::string instrumentName;
    bool fileLog, consoleLog;
    std::string url;
    std::map<int, std::string> prevData;
    std::shared_ptr<spdlog::logger> fileLogger;
    std::shared_ptr<spdlog::logger> consoleLogger;

public:
    void setSymbolName(std::string symName);
    void setstatus(std::string stat);
    void setContractType(std::string CType);
    void setTicksize(std::string TSize);
    void setStepSize(std::string SSize);

    std::string getSymbolName();
    std::string getContractType();
    std::string getStatus();
    std::string getTickSize();
    std::string getStepSize();
  
    int configFunc();
    // void getExchangeInfo();
    void readQueryFile();
    void queryCheck(const rapidjson::Value &queryContent);
    void deleteData(int id, std::string instrumentName);
    // void getData(const std::string &instrumentName);
    void updatetData(const rapidjson::Value &queryObject);
    // void getBoostExchangeInfo();
    void getBoostStruct();
    void getDataStruct(const std::string &instrumentName, const rapidjson::Value &queryContent);

};

#endif // EXCHANGE_INFO_H
