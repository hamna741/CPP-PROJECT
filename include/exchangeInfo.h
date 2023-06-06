/*
#include <iostream>
#include <string>
#include <regex>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <thread>
#include <chrono>
#include <time.h>
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/basic_file_sink.h"

namespace exchangeInfo
{
    class exchangeInfoClass
    {
    private:
        std::string jsonData;
        int id;
        std::vector<int> prevID;
        std::string instrumentName;
        std::vector<std::string> prevInstrumentName;
        std::map<int, std::string> prevData;
        std::string contractType;
        std::string tickSize;
        std::string stepSize;
        std::string filterType;
        std::string status;
        bool fileLog, consoleLog;
        std::shared_ptr<spdlog::logger> fileLogger;
        std::shared_ptr<spdlog::logger> consoleLogger;

    public:
        exchangeInfoClass();
        int readQueryFile();
        void getExchangeInfo();
        void deleteData(int id, std::string instrumentName);
        void getData(std::string instrumentName);
        void updatetData(const rapidjson::Value &queryObject);
        void queryCheck(std::string &queryContent);
        void configFunc();
        void setlogger();
        ~exchangeInfoClass();
    };
}


*/
#ifndef EXCHANGE_INFO_H
#define EXCHANGE_INFO_H

#include <vector>
#include <iostream>
#include <string>
#include <regex>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <thread>
#include <chrono>
#include <time.h>
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/basic_file_sink.h"
#include <boost/asio.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/version.hpp>
// namespace beast = boost::beast;         // from <boost/beast.hpp>
// namespace http = beast::http; 
using boost::asio::ip::tcp;
class ExchangeInfoClass
{
private:
    std::map<std::string, rapidjson::Value> symbolDataMap;
    struct SymbolData {
    std::string symbolName;
    std::string status;
    std::string contractType;
    std::string tickSize;
    std::string stepSize;
};
    int id;
     std::vector<SymbolData> symbolDataList;
    std::string instrumentName;
    bool fileLog, consoleLog;
    std::string url;
    std::map<int, std::string> prevData;
    std::shared_ptr<spdlog::logger> fileLogger;
    std::shared_ptr<spdlog::logger> consoleLogger;

public:
    //  ExchangeInfoClass();
    int configFunc();
    void getExchangeInfo();
    void readQueryFile();
    void queryCheck(const rapidjson::Value &queryContent);
    void deleteData(int id, std::string instrumentName);
    void getData(const std::string &instrumentName);
    void updatetData(const rapidjson::Value &queryObject);
    void getBoostExchangeInfo();
    void getBoostStruct();
    void getDataStruct(const std::string& instrumentName, const rapidjson::Value& queryContent);
};

#endif // EXCHANGE_INFO_H
