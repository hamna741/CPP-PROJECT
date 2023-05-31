#include "exchangeInfo.h"

int ExchangeInfoClass::configFunc()
{
    std::string logLevel;
 int interval;
    std::string prevQueryData;
    std::ifstream configFile("/home/hamna/Desktop/myproject/CPP-PROJECT/files/configFile.json");

    if (!configFile.is_open())
    {
        std::cout << "Failed to open config the file." << std::endl;
    }
    rapidjson::Document configDoc;
    rapidjson::IStreamWrapper exchangeInfoStreamWrapper(configFile);
    configDoc.ParseStream(exchangeInfoStreamWrapper);

    configFile.close();

    if (configDoc.HasParseError())
    {
        std::cout << "Failed to parse JSON response." << std::endl;
        exit(1);
    }
    if (configDoc.HasMember("logging") && configDoc["logging"].IsObject())
    {
        const rapidjson::Value &loggingConfig = configDoc["logging"].GetObject();
        if (loggingConfig.HasMember("level") && loggingConfig["level"].IsString())
        {
            logLevel = loggingConfig["level"].GetString();
            spdlog::set_level(spdlog::level::from_str(logLevel));
        }
        if (loggingConfig.HasMember("file") && loggingConfig["file"].IsBool())
        {
            fileLog = loggingConfig["file"].GetBool();
            if (fileLog)
            {
                std::cout << "call setfilelog" << std::endl;
                fileLogger = spdlog::basic_logger_mt("file_logger", "/home/hamna/Desktop/myproject/CPP-PROJECT/build/log_file.txt");
                spdlog::set_default_logger(fileLogger);
            }
        }
        if (loggingConfig.HasMember("console") && loggingConfig["console"].IsBool())
        {
            consoleLog = loggingConfig["console"].GetBool();
            if (consoleLog)
            {
                std::cout << "call consolelog" << std::endl;
                consoleLogger = spdlog::stdout_color_mt("console");
                spdlog::set_default_logger(consoleLogger);
            }
        }
    }
    if (configDoc.HasMember("exchange_info_url") && configDoc["exchange_info_url"].IsString())
    {
         url=configDoc["exchange_info_url"].GetString();
         
    }
    if (configDoc.HasMember("request_interval") && configDoc["request_interval"].IsInt())
    {
         interval = configDoc["request_interval"].GetInt();
    }
    return interval;
}
void ExchangeInfoClass::getExchangeInfo()
{
    web::http::client::http_client client(U(url));

    web::uri_builder builder(U("/fapi/v1/exchangeInfo"));
    client.request(web::http::methods::GET, builder.to_string())
        .then([](web::http::http_response response)
              {
            if (response.status_code() == web::http::status_codes::OK) {
               
                return response.extract_string();
            } else {
                throw std::runtime_error("Request failed with status code: " + std::to_string(response.status_code()));
            } })
        .then([this](std::string body)
              {
            rapidjson::Document document;
            document.Parse(body.c_str());

            if (!document.HasParseError()) {
               
                std::map<std::string, rapidjson::Value> symbolData;

                const rapidjson::Value& symbols = document["symbols"];
                if (symbols.IsArray()) {
                    for (rapidjson::SizeType i = 0; i < symbols.Size(); i++) {
                        const rapidjson::Value& symbol = symbols[i];

                        std::string symbolName = symbol["symbol"].GetString();

                        rapidjson::Value dataObject(rapidjson::kObjectType);

                        if (symbol.HasMember("status") && symbol["status"].IsString()) {
                            std::string status = symbol["status"].GetString();
                            dataObject.AddMember("status", rapidjson::Value(status.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                        }

                        if (symbol.HasMember("contractType") && symbol["contractType"].IsString()) {
                            std::string contractType = symbol["contractType"].GetString();
                            dataObject.AddMember("contractType", rapidjson::Value(contractType.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                        }

                        if (symbol.HasMember("filters") && symbol["filters"].IsArray()) {
                            const rapidjson::Value& filters = symbol["filters"];

                            for (rapidjson::SizeType j = 0; j < filters.Size(); j++) {
                                const rapidjson::Value& filter = filters[j];

                                if (filter.HasMember("filterType") && filter["filterType"].IsString()) {
                                    std::string filterType = filter["filterType"].GetString();

                                    if (filterType == "PRICE_FILTER") {
                                        if (filter.HasMember("tickSize") && filter["tickSize"].IsString()) {
                                            std::string tickSize = filter["tickSize"].GetString();
                                            dataObject.AddMember("tickSize", rapidjson::Value(tickSize.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                                        }
                                    } else if (filterType == "LOT_SIZE") {
                                        if (filter.HasMember("stepSize") && filter["stepSize"].IsString()) {
                                            std::string stepSize = filter["stepSize"].GetString();
                                            dataObject.AddMember("stepSize", rapidjson::Value(stepSize.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                                        }
                                    }
                                }
                            }
                        }

                        symbolData[symbolName] = dataObject;
                    }
                }

                symbolDataMap = std::move(symbolData);

                if (consoleLog) {
                    consoleLogger->info("Exchange info retrieved successfully");
                }
            } else {
                if (fileLog) {
                    fileLogger->error("Failed to parse exchange info JSON");
                    fileLogger->flush();
                }

                if (consoleLog) {
                    consoleLogger->error("Failed to parse exchange info JSON");
                }
            } })
        .wait();
}
void ExchangeInfoClass::readQueryFile()
{
    std::string prevQueryData;
    std::ifstream inputFile("/home/hamna/Desktop/myproject/CPP-PROJECT/files/queryfile.json");

    if (!inputFile.is_open())
    {
        std::cout << "Failed to open the file." << std::endl;
        exit;
    }

    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::string currentData(fileSize, ' ');
    inputFile.read(&currentData[0], fileSize);
    inputFile.close();

    while (true)
    {
        std::ifstream inputFile("/home/hamna/Desktop/myproject/CPP-PROJECT/files/queryfile.json");

        if (!inputFile.is_open())
        {
            std::cout << "Failed to open the file." << std::endl;
            if (fileLog)
                fileLogger->error("failed to open query.json file");
            if (consoleLog)
                fileLogger->error("failed to open query.json file");
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second before retrying
            continue;
        }

        inputFile.seekg(0, std::ios::end);
        fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        std::string newData(fileSize, ' ');
        inputFile.read(&newData[0], fileSize);
        inputFile.close();

        if (newData != prevQueryData)
        {
             
            rapidjson::Document document;
            document.Parse(newData.c_str());

            if (!document.HasParseError() && document.IsObject())
            {
              
                if (document.HasMember("query") && document["query"].IsArray())
                {
                    const rapidjson::Value& queryArray = document["query"];
                    for (rapidjson::SizeType i = 0; i < queryArray.Size(); i++)
                    {
                        const rapidjson::Value& queryData = queryArray[i];
                   
                        
                        queryCheck(queryData);
                    }
                }
            }
            else
            {
                std::cout << "Failed to parse the JSON data." << std::endl;
                if (fileLog)
                    fileLogger->error("failed to parse JSON data");
                if (consoleLog)
                    fileLogger->error("failed to parse JSON data");
            }

            prevQueryData = newData;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void ExchangeInfoClass::queryCheck(const rapidjson::Value& queryContent)
{
    if (queryContent.IsObject())
    {
        if (queryContent.HasMember("id") && queryContent["id"].IsInt())
        {
            id = queryContent["id"].GetInt();
            std::cout << "ID: " << id << std::endl;
        }

        if (queryContent.HasMember("instrument_name") && queryContent["instrument_name"].IsString())
        {
            instrumentName = queryContent["instrument_name"].GetString();
            std::cout << "Instrument Name: " << instrumentName << std::endl;
        }

        if (queryContent.HasMember("query_type") && queryContent["query_type"].IsString())
        {
            std::string queryType = queryContent["query_type"].GetString();
            std::cout << "Query Type: " << queryType << std::endl;

            if (queryType == "GET")
            {
                if (fileLog)
                {
                    fileLogger->info("Query type is GET");
                    fileLogger->flush();
                }
                if (consoleLog)
                {
                    consoleLogger->debug("Query type is GET");
                }

                if (prevData[id] == instrumentName)
                {
                    std::cout << "Request repeated" << std::endl;
                }
                else
                {
                    prevData[id] = instrumentName;
                    ExchangeInfoClass::getData(instrumentName);
                }
            }
            else if (queryType == "UPDATE")
            {
                ExchangeInfoClass::updatetData(queryContent);
            }
            else if (queryType == "DELETE")
            {
                ExchangeInfoClass::deleteData(id, instrumentName);
            }
            else
            {
                if (fileLog)
                {
                    fileLogger->error("Unknown query type: " + queryType);
                    fileLogger->flush();
                }
                if (consoleLog)
                {
                    consoleLogger->error("Unknown query type: " + queryType);
                }
            }
        }

        if (queryContent.HasMember("data"))
        {
            const rapidjson::Value& data = queryContent["data"];

            if (data.IsArray())
            {
                std::cout << "Data:" << std::endl;
                for (rapidjson::SizeType j = 0; j < data.Size(); j++)
                {
                    if (data[j].IsString())
                    {
                        std::string value = data[j].GetString();
                        std::cout << value << std::endl;
                    }
                }
            }
            else if (data.IsObject())
            {
                std::cout << "Data:" << std::endl;
                for (rapidjson::Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); ++itr)
                {
                    if (itr->value.IsString())
                    {
                        std::string key = itr->name.GetString();
                        std::string value = itr->value.GetString();
                        std::cout << key << ": " << value << std::endl;
                    }
                }
            }
        }

        std::cout << std::endl;
    }
}

void ExchangeInfoClass::getData(const std::string &instrumentName)
{
    std::cout << "QUERY TYPE IS GET" << std::endl;


    if (symbolDataMap.find(instrumentName) != symbolDataMap.end())
    {
        const rapidjson::Value &symbolData = symbolDataMap[instrumentName];

        rapidjson::Document queryDoc;
        std::ifstream ansFile("/home/hamna/Desktop/myproject/CPP-PROJECT/files/answers.json");
        if (ansFile.is_open())
        {
            rapidjson::IStreamWrapper ansStreamWrapper(ansFile);
            queryDoc.ParseStream(ansStreamWrapper);
            ansFile.close();
        }
        else
        {
            queryDoc.SetObject();
        }

        rapidjson::Document::AllocatorType &allocator = queryDoc.GetAllocator();

        if (!queryDoc.HasMember("answers"))
        {
            rapidjson::Value answers(rapidjson::kArrayType);
            queryDoc.AddMember("answers", answers, allocator);
        }

        rapidjson::Value answersArray = queryDoc["answers"].GetArray();

        rapidjson::Value dataObject(symbolData, allocator);

        rapidjson::Value result(rapidjson::kObjectType);
        std::string str = std::to_string(id);
        rapidjson::Value strId(str.c_str(), str.size(), allocator); 
        result.AddMember("ID", rapidjson::Value(strId, allocator).Move(), allocator);
        result.AddMember("symbol", rapidjson::Value(instrumentName.c_str(), allocator).Move(), allocator);
        result.AddMember("data", dataObject, allocator); 

        answersArray.PushBack(result, allocator);

        queryDoc["answers"] = answersArray;

        std::ofstream ansFileWrite("/home/hamna/Desktop/myproject/CPP-PROJECT/files/answers.json");
        if (!ansFileWrite.is_open())
        {
            if (fileLog)
            {
                fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
                fileLogger->flush();
            }

            if (consoleLog)
            {
                consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
            }
            exit(1);
        }

        rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
        queryDoc.Accept(answersWriter);

        ansFileWrite.close();

        if (consoleLog)
        {
            consoleLogger->error("Data written in file successfully");
        }
    }
    else
    {
        std::cout << "Symbol not found." << std::endl;
    }
}

void ExchangeInfoClass::deleteData(int id, std::string instrumentName)
{

    std::ifstream ansFile("/home/hamna/Desktop/myproject/CPP-PROJECT/files/answers.json");
    if (!ansFile.is_open())
    {
        if (fileLog)
        {
            fileLogger->error("ENCOUNTERED ERROR WHILE OPEING answers.json FILE");
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->error("ENCOUNTERED ERROR WHILE OPEING answers.json FILE");
        }
        exit(1);
    }

    rapidjson::Document answerDoc;
    rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
    answerDoc.ParseStream(ansFileStreamWrapper);

    ansFile.close();

    if (answerDoc.HasParseError())
    {
        if (fileLog)
        {
            fileLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
        }
        exit(1);
    }

    if (answerDoc.IsObject() && answerDoc.HasMember("answers"))
    {
        rapidjson::Value &answersArray = answerDoc["answers"];
        rapidjson::SizeType index = 0;

        while (index < answersArray.Size())
        {
            if (answersArray[index].IsObject())
            {
                const rapidjson::Value &answerObject = answersArray[index];

                if (answerObject.HasMember("ID") && answerObject["ID"].IsString() && answerObject.HasMember("symbol") && answerObject["symbol"].IsString())
                {

                    std::string ansID = answerObject["ID"].GetString();
                    std::string ansSymbol = answerObject["symbol"].GetString();

                    if (std::stoi(ansID) == id && ansSymbol == instrumentName)
                    {

                        if (consoleLog)
                        {
                            consoleLogger->debug("deleting data");
                        }
                        answersArray.Erase(answersArray.Begin() + index);
                    }
                    else
                    {

                        index++;
                    }
                }
            }
            else
            {

                index++;
            }
        }
    }

    std::ofstream ansFileWrite("/home/hamna/Desktop/myproject/CPP-PROJECT/files/answers.json");
    if (!ansFileWrite.is_open())
    {
        if (fileLog)
        {
            fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
        }
        exit(1);
    }

    rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
    answerDoc.Accept(answersWriter);

    ansFileWrite.close();
}

void ExchangeInfoClass::updatetData(const rapidjson::Value &queryObject)
{
    if (queryObject.HasMember("id") && queryObject["id"].IsInt() && queryObject.HasMember("instrument_name") && queryObject["instrument_name"].IsString() && queryObject.HasMember("data") && queryObject["data"].IsObject())
    {
        int id = queryObject["id"].GetInt();
        std::string instrumentName = queryObject["instrument_name"].GetString();
        const rapidjson::Value &newData = queryObject["data"];


        std::ifstream ansFile("/home/hamna/Desktop/myproject/CPP-PROJECT/files/answers.json");

        if (!ansFile.is_open())
        {
           if (fileLog){
            fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
        fileLogger->flush();
        }

        if (consoleLog){
            consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
        }
            exit(1);
        }

        rapidjson::Document answerDoc;
        rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
        answerDoc.ParseStream(ansFileStreamWrapper);

        ansFile.close();

        if (answerDoc.HasParseError())
        {
           if (fileLog){
            fileLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
        fileLogger->flush();
        }

        if (consoleLog){
            consoleLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
        }
            exit(1);
        }

        if (answerDoc.IsObject() && answerDoc.HasMember("answers"))
        {
            rapidjson::Value &answersArray = answerDoc["answers"];
            rapidjson::SizeType index = 0;

            while (index < answersArray.Size())
            {
                if (answersArray[index].IsObject())
                {
                    rapidjson::Value &answerObject = answersArray[index];

                    if (answerObject.HasMember("ID") && answerObject["ID"].IsString() &&
                        answerObject.HasMember("symbol") && answerObject["symbol"].IsString())
                    {
                        std::string ansID = answerObject["ID"].GetString();
                        std::string ansSymbol = answerObject["symbol"].GetString();

                        if (std::stoi(ansID) == id && ansSymbol == instrumentName)
                        {

                            rapidjson::Value newDataValue(newData, answerDoc.GetAllocator());
                            answerObject.RemoveMember("data");
                            answerObject.AddMember("data", newDataValue, answerDoc.GetAllocator());
                        }
                    }
                }

                index++;
            }
        }

        std::ofstream ansFileWrite("/home/hamna/Desktop/myproject/CPP-PROJECT/files/answers.json");
        if (!ansFileWrite.is_open())
        {
            if (fileLog){
            fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
        fileLogger->flush();
        }

        if (consoleLog){
            consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
        }
            exit(1);
        }

        rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
        answerDoc.Accept(answersWriter);

        ansFileWrite.close();

       if (fileLog){
            fileLogger->info("data updated in file");
        fileLogger->flush();
        }

        if (consoleLog){
            consoleLogger->info("data updated in file");
        }
    }
    else
    {
        if (fileLog){
            fileLogger->error("INVALID DATA");
        fileLogger->flush();
        }

        if (consoleLog){
            consoleLogger->error("INVALID DATA");
        }
    }
}
