#include "exchangeInfo.h"
// namespace binance
// {
void ExchangeInfoClass::setContractType(std::string CType)
{
    dataObject.contractType = CType;
}

void ExchangeInfoClass::setstatus(std::string stat)
{
    dataObject.status = stat;
}

void ExchangeInfoClass::setStepSize(std::string SSize)
{
    dataObject.stepSize = SSize;
}

void ExchangeInfoClass::setTicksize(std::string TSize)
{
    dataObject.tickSize = TSize;
}

void ExchangeInfoClass::setSymbolName(std::string symName)
{
    dataObject.symbolName = symName;
}

std::string ExchangeInfoClass::getContractType()
{
    return dataObject.contractType;
}

std::string ExchangeInfoClass::getStatus()
{
    return dataObject.status;
}

std::string ExchangeInfoClass::getStepSize()
{
    return dataObject.stepSize;
}

std::string ExchangeInfoClass::getTickSize()
{
    return dataObject.tickSize;
}

std::string ExchangeInfoClass::getSymbolName()
{
    return dataObject.symbolName;
}
int ExchangeInfoClass::configFunc()
{
    std::string logLevel;
    int interval;
    std::string prevQueryData;
    std::ifstream configFile("/CPP-PROJECT/files/configFile.json");

    if (!configFile.is_open())
    {
        throw std::runtime_error("Failed to open config the file.");
    }
    rapidjson::Document configDoc;
    rapidjson::IStreamWrapper exchangeInfoStreamWrapper(configFile);
    configDoc.ParseStream(exchangeInfoStreamWrapper);

    configFile.close();

    if (configDoc.HasParseError())
    {
        throw std::runtime_error("Failed to parse JSON response.");
        exit(1);
    }
    if (configDoc.HasMember("logging") && configDoc["logging"].IsObject())
    {
        const rapidjson::Value &loggingConfig = configDoc["logging"].GetObject();
        if (loggingConfig.HasMember("level") && loggingConfig["level"].IsString())
        {
            logLevel = loggingConfig["level"].GetString();
            // spdlog::set_level(spdlog::level::from_str(logLevel));
        }
        if (loggingConfig.HasMember("file") && loggingConfig["file"].IsBool())
        {
            fileLog = loggingConfig["file"].GetBool();
            fileLogger = spdlog::basic_logger_mt("file_logger", "/CPP-PROJECT/files/logFile.txt");
            spdlog::set_default_logger(fileLogger);
            if (fileLog)
            {
                fileLogger->set_level(spdlog::level::from_str(logLevel));
            }
            else
            {
                fileLogger->set_level(spdlog::level::off);
            }
        }
        if (loggingConfig.HasMember("console") && loggingConfig["console"].IsBool())
        {
            consoleLog = loggingConfig["console"].GetBool();
            consoleLogger = spdlog::stdout_color_mt("console");
             spdlog::set_default_logger(consoleLogger);
            if (consoleLog)
            {
                consoleLogger->set_level(spdlog::level::from_str(logLevel));
            }
            else
                consoleLogger->set_level(spdlog::level::off);
        }
    }
    if (configDoc.HasMember("exchange_info_url") && configDoc["exchange_info_url"].IsString())
    {
        url = configDoc["exchange_info_url"].GetString();
    }
    if (configDoc.HasMember("request_interval") && configDoc["request_interval"].IsInt())
    {
        interval = configDoc["request_interval"].GetInt();
    }
    return interval;
}

void ExchangeInfoClass::getExchangeInfo()
{

    boost::asio::io_context io;
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.set_verify_mode(boost::asio::ssl::verify_peer);

    ctx.add_verify_path("/etc/ssl/certs");

    ctx.set_options(boost::asio::ssl::context::default_workarounds |
                    boost::asio::ssl::context::no_sslv2 |
                    boost::asio::ssl::context::no_sslv3);
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io,
                                                                  ctx);
    boost::asio::ip::tcp::resolver resolver(io);

    boost::system::error_code err;
    size_t slashPos = url.find('/');
    std::string domain = url.substr(0, slashPos);
    std::string endPoint = url.substr(slashPos);
    boost::asio::ip::tcp::resolver::query query(domain, "https");

    auto const results = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = results;

    boost::asio::ip::tcp::resolver::iterator end;

    boost::asio::connect(socket.lowest_layer(), endpoint_iterator, end, err);
    if (!err){
        
            fileLogger->debug("CONNECTION ESTABLISHED");
            fileLogger->flush();
   
        consoleLogger->debug("CONNECTION ESTABLISHED ");}
  
    else
    {
       
            fileLogger->error("CONNECTION FAILED");
            fileLogger->flush();
      
            consoleLogger->error("CONNECTION FAILED ");
  
    }

    boost::asio::streambuf request;
    socket.handshake(boost::asio::ssl::stream_base::client);
    {
        std::ostream request_stream(&request);

        request_stream << "GET "
                       << endPoint
                       << " HTTP/1.1\r\n";

        request_stream << "Host: " << domain << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
    }

    boost::asio::write(socket, request);

    boost::asio::streambuf response_;
    boost::system::error_code ec;
    read(socket, response_, ec);
    std::istream response_stream(&response_);

    std::string http_version;
    unsigned int status_code;
    response_stream >> http_version >> status_code;
    try
    {
        if (status_code == 200)
        {
            boost::asio::read_until(socket, response_, "\r\n\r\n");
            std::string response_body;
            std::ostringstream response_body_stream;
            response_body_stream << &response_;
            response_body = response_body_stream.str();

            size_t content_start = response_body.find("\r\n\r\n");
            std::string response_content = response_body.substr(content_start);

            rapidjson::Document document;
            document.Parse(response_content.c_str());

            if (!document.HasParseError())
            {

                const rapidjson::Value &symbols = document["symbols"];
                if (symbols.IsArray())
                {
                    for (rapidjson::SizeType i = 0; i < symbols.Size(); i++)
                    {
                        const rapidjson::Value &symbol = symbols[i];
                        // SymbolData dataObject;
                        ExchangeInfoClass::setSymbolName(symbol["symbol"].GetString());

                        if (symbol.HasMember("status") && symbol["status"].IsString())
                        {
                            ExchangeInfoClass::setstatus(symbol["status"].GetString());
                        }

                        if (symbol.HasMember("contractType") && symbol["contractType"].IsString())
                        {
                            ExchangeInfoClass::setContractType(symbol["contractType"].GetString());
                        }

                        if (symbol.HasMember("filters") && symbol["filters"].IsArray())
                        {
                            const rapidjson::Value &filters = symbol["filters"];

                            for (rapidjson::SizeType j = 0; j < filters.Size(); j++)
                            {
                                const rapidjson::Value &filter = filters[j];

                                if (filter.HasMember("filterType") && filter["filterType"].IsString())
                                {
                                    std::string filterType = filter["filterType"].GetString();

                                    if (filterType == "PRICE_FILTER")
                                    {
                                        if (filter.HasMember("tickSize") && filter["tickSize"].IsString())
                                        {
                                            ExchangeInfoClass::setTicksize(filter["tickSize"].GetString());
                                        }
                                    }
                                    else if (filterType == "LOT_SIZE")
                                    {
                                        if (filter.HasMember("stepSize") && filter["stepSize"].IsString())
                                        {
                                            ExchangeInfoClass::setStepSize(filter["stepSize"].GetString());
                                        }
                                    }
                                }
                            }
                        }

                        symbolDataVec.push_back(dataObject);
                    }

                  
                        consoleLogger->info("Exchange info retrieved successfully");
                   
                        fileLogger->info("Exchange info retrieved successfully");
                 
                }
            }
        }
        else
        {
            throw ::std::runtime_error("Request failed with status code: " + std::to_string(status_code));
            
                fileLogger->error("Request failed with status code: " + std::to_string(status_code));
                fileLogger->flush();
         
                consoleLogger->error("Request failed with status code: " + std::to_string(status_code));
                exit(1);
         
        }
    }
    catch (const char *msg)
    {
        std::cout << msg << std::endl;
    }
}

void ExchangeInfoClass::readQueryFile()
{
    std::string prevQueryData;
    std::ifstream inputFile("/CPP-PROJECT/files/queryfile.json");
    try
    {
        if (!inputFile.is_open())
        {
            throw std::runtime_error("Failed to open the file.");
        }
    }
    catch (const char *msg)
    {
        std::cout << msg << std::endl;
    }

    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::string currentData(fileSize, ' ');
    inputFile.read(&currentData[0], fileSize);
    inputFile.close();

    while (true)
    {
        std::ifstream inputFile("/CPP-PROJECT/files/queryfile.json");

        if (!inputFile.is_open())
        {
            throw std::runtime_error("Failed to open the file.");
          
                fileLogger->error("failed to open query.json file");
          
                fileLogger->error("failed to open query.json file");
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
                    const rapidjson::Value &queryArray = document["query"];
                    for (rapidjson::SizeType i = 0; i < queryArray.Size(); i++)
                    {
                        const rapidjson::Value &queryData = queryArray[i];

                        queryCheck(queryData);
                    }
                }
            }
            else
            {
                throw std::runtime_error("Failed to parse the JSON data.");
           
                    fileLogger->error("failed to parse JSON data");
               
                    fileLogger->error("failed to parse JSON data");
            }

            prevQueryData = newData;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ExchangeInfoClass::queryCheck(const rapidjson::Value &queryContent)
{
    int id;
    if (queryContent.IsObject())
    {
        if (queryContent.HasMember("id") && queryContent["id"].IsInt())
        {
            id = queryContent["id"].GetInt();
        }

        if (queryContent.HasMember("instrument_name") && queryContent["instrument_name"].IsString())
        {
            instrumentName = queryContent["instrument_name"].GetString();
        }

        if (queryContent.HasMember("query_type") && queryContent["query_type"].IsString())
        {
            std::string queryType = queryContent["query_type"].GetString();

            if (queryType == "GET")
            {
           
                    fileLogger->info("Query type is GET");
                    fileLogger->flush();
             
                    consoleLogger->debug("Query type is GET");
           

                if (prevData[id] == instrumentName)
                {
                
                        fileLogger->info("Request repeated");
                        fileLogger->flush();
                   
                        consoleLogger->debug("Request repeated");
              
                }
                else
                {
                    prevData[id] = instrumentName;
                    ExchangeInfoClass::getData(instrumentName, queryContent);
                }
            }
            else if (queryType == "UPDATE")
            {
               
                    fileLogger->info("Query type is UPDATE");
                    fileLogger->flush();
            
                    consoleLogger->debug("Query type is UPDATE");
        
                ExchangeInfoClass::updatetData(queryContent);
            }
            else if (queryType == "DELETE")
            {
               
                    fileLogger->info("Query type is DELETE");
                    fileLogger->flush();
            
            
                ExchangeInfoClass::deleteData(id, instrumentName);
            }
            else
            {
             
                    fileLogger->error("Unknown query type: " + queryType);
                    fileLogger->flush();
           
                    consoleLogger->error("Unknown query type: " + queryType);
                
            }
        }
    }
}

void ExchangeInfoClass::getData(const std::string &instrumentName, const rapidjson::Value &queryContent)
{

    auto symbolDataIter = symbolDataVec.end();
    for (auto iter = symbolDataVec.begin(); iter != symbolDataVec.end(); ++iter)
    {
        if (iter->symbolName == instrumentName)
        {
            symbolDataIter = iter;
            break;
        }
    }
    try
    {
        if (symbolDataIter != symbolDataVec.end())
        {

            rapidjson::Document queryDoc;
            std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
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

            rapidjson::Value dataObject(rapidjson::kObjectType);
            int Nid = queryContent["id"].GetInt();
            if (queryContent.HasMember("data") && queryContent["data"].IsArray())
            {
                const rapidjson::Value &dataFields = queryContent["data"];
                for (rapidjson::SizeType i = 0; i < dataFields.Size(); i++)
                {
                    const std::string &fieldName = dataFields[i].GetString();

                    if (fieldName == "status" && !ExchangeInfoClass::getStatus().empty())
                    {
                        dataObject.AddMember("status", rapidjson::Value(ExchangeInfoClass::getStatus().c_str(), allocator).Move(), allocator);
                    }
                    else if (fieldName == "contract_type" && !ExchangeInfoClass::getContractType().empty())
                    {
                        dataObject.AddMember("contract_type", rapidjson::Value(ExchangeInfoClass::getContractType().c_str(), allocator).Move(), allocator);
                    }
                    else if (fieldName == "ticksize" && !ExchangeInfoClass::getTickSize().empty())
                    {
                        dataObject.AddMember("ticksize", rapidjson::Value(ExchangeInfoClass::getTickSize().c_str(), allocator).Move(), allocator);
                    }
                    else if (fieldName == "stepsize")
                    {
                        dataObject.AddMember("stepsize", rapidjson::Value(ExchangeInfoClass::getStepSize().c_str(), allocator).Move(), allocator);
                    }
                }
            }

            rapidjson::Value result(rapidjson::kObjectType);
            std::string str = std::to_string(Nid);
            result.AddMember("ID", rapidjson::Value(str.c_str(), allocator).Move(), allocator);
            result.AddMember("symbol", rapidjson::Value(instrumentName.c_str(), allocator).Move(), allocator);
            result.AddMember("data", dataObject, allocator);

            answersArray.PushBack(result, allocator);

            queryDoc["answers"] = answersArray;

            std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
            if (!ansFileWrite.is_open())
            {
              
                    fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
                    fileLogger->flush();
           

              
                    consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
              
                throw std::runtime_error("CANNOT OPEN FILE");
            }

            rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
            rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
            queryDoc.Accept(answersWriter);

            ansFileWrite.close();

         
                consoleLogger->debug("Data written to the file successfully");
       
        }
        else
        {
            throw std::runtime_error("Symbol not found.");
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}

void ExchangeInfoClass::deleteData(int id, std::string instrumentName)
{

    std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
    if (!ansFile.is_open())
    {
   
            fileLogger->error("ENCOUNTERED ERROR WHILE OPEING answers.json FILE");
            fileLogger->flush();
      
            consoleLogger->error("ENCOUNTERED ERROR WHILE OPEING answers.json FILE");
    
        exit(1);
    }

    rapidjson::Document answerDoc;
    rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
    answerDoc.ParseStream(ansFileStreamWrapper);

    ansFile.close();

    if (answerDoc.HasParseError())
    {
      
            fileLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
            fileLogger->flush();
       

     
            consoleLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
   
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

                   
                            consoleLogger->debug("deleting data");
                     
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

    std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
    if (!ansFileWrite.is_open())
    {
     
            fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
            fileLogger->flush();


 
            consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
   
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

        std::ifstream ansFile("/CPP-PROJECT/files/answers.json");

        if (!ansFile.is_open())
        {
        
                fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
                fileLogger->flush();
   
                consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
  
            exit(1);
        }

        rapidjson::Document answerDoc;
        rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
        answerDoc.ParseStream(ansFileStreamWrapper);

        ansFile.close();

        if (answerDoc.HasParseError())
        {
          
                fileLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
                fileLogger->flush();
      
                consoleLogger->error("ENCOUNTERED ERROR WHILE PARSING DATA");
     
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
                            if (answerObject.HasMember("data") && answerObject["data"].IsObject())
                            {
                                rapidjson::Value &dataObject = answerObject["data"];
                                for (auto it = newData.MemberBegin(); it != newData.MemberEnd(); ++it)
                                {

                                    if (dataObject.HasMember(it->name))
                                    {
                                        dataObject[it->name].CopyFrom(it->value, answerDoc.GetAllocator());
                                        // dataObject[it->name] = it->value;
                                    }
                                }
                            }

                            //     rapidjson::Value newDataValue(newData, answerDoc.GetAllocator());

                            //    answerObject.RemoveMember("data");
                            //     answerObject.AddMember("data", newDataValue, answerDoc.GetAllocator());
                        }
                    }
                }

                index++;
            }
        }

        std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
        if (!ansFileWrite.is_open())
        {
         
                fileLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
                fileLogger->flush();
     

       
                consoleLogger->error("ENCOUNTERED ERROR WHILE OPENING answers.json FILE");
    
            exit(1);
        }

        rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
        answerDoc.Accept(answersWriter);

        ansFileWrite.close();

     
            fileLogger->info("data updated in file");
            fileLogger->flush();
    
            consoleLogger->info("data updated in file");
  
    }
    else
    {
       
            fileLogger->error("INVALID DATA");
            fileLogger->flush();
       
            consoleLogger->error("INVALID DATA");
  
    }
}
//}
