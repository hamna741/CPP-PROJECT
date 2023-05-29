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

class ExchangeInfoClass
{
private:
    std::map<std::string, rapidjson::Value> symbolDataMap;
    int id;
    std::string instrumentName;
    bool fileLog, consoleLog;
    std::map<int, std::string> prevData;
    std::shared_ptr<spdlog::logger> fileLogger;
    std::shared_ptr<spdlog::logger> consoleLogger;

public:
    //  ExchangeInfoClass();
    void configFunc();
    void getExchangeInfo();
    void readQueryFile();
    void queryCheck(std::string &queryContent);
    void deleteData(int id, std::string instrumentName);
    void getData(const std::string& instrumentName);
    void updatetData(const rapidjson::Value &queryObject);
   
};

#endif // EXCHANGE_INFO_H
