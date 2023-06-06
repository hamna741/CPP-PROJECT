#include "exchangeInfo.h"

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
            spdlog::set_level(spdlog::level::from_str(logLevel));
        }
        if (loggingConfig.HasMember("file") && loggingConfig["file"].IsBool())
        {
            fileLog = loggingConfig["file"].GetBool();
            if (fileLog)
            {

                fileLogger = spdlog::basic_logger_mt("file_logger", "/CPP-PROJECT/build/log_file.txt");
                spdlog::set_default_logger(fileLogger);
            }
        }
        if (loggingConfig.HasMember("console") && loggingConfig["console"].IsBool())
        {
            consoleLog = loggingConfig["console"].GetBool();
            if (consoleLog)
            {

                consoleLogger = spdlog::stdout_color_mt("console");
                spdlog::set_default_logger(consoleLogger);
            }
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

void ExchangeInfoClass::getBoostExchangeInfo()
{
    boost::asio::io_context io;
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.set_verify_mode(boost::asio::ssl::verify_peer);
    // ctx.load_verify_file("ca.pem");
    ctx.add_verify_path("/etc/ssl/certs");

    ctx.set_options(boost::asio::ssl::context::default_workarounds |
                    boost::asio::ssl::context::no_sslv2 |
                    boost::asio::ssl::context::no_sslv3);
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io,
                                                                  ctx);
    boost::asio::ip::tcp::resolver resolver(io);
    // boost::asio::ip::tcp::socket socket(io);
    boost::system::error_code err;
    boost::asio::ip::tcp::resolver::query query(url, "https");
    std::cout << "1" << std::endl;
    auto const results = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = results;
    // boost::asio::ip::tcp::endpoint connectionEndpoint(endpoint_iterator->address(), 80);
    std::cout << "2" << std::endl;
    boost::asio::ip::tcp::resolver::iterator end;

    boost::asio::connect(socket.lowest_layer(), endpoint_iterator, end, err);
    if (!err)
        std::cout << "connected" << std::endl;
    else
        std::cout << "not connected" << std::endl;
    // boost::asio::connect(socket, endpoint_iterator);

    boost::asio::streambuf request;
    socket.handshake(boost::asio::ssl::stream_base::client);
    {
        std::ostream request_stream(&request);

        request_stream << "GET "
                       << "/fapi/v1/exchangeInfo"
                       << " HTTP/1.1\r\n";
        // request_stream << "GET " << "/fapi/v1/exchangeInfo" << " HTTPS/1.0\r\n";  // note that you can change it if you wish to HTTP/1.0
        request_stream << "Host: " << url << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
    }

    boost::asio::write(socket, request);

    boost::asio::streambuf response_;
    boost::system::error_code ec;
    read(socket, response_, ec);
    std::istream response_stream(&response_);
    // std::cout << &response_ << "\n";
    //  std::cout << &response_ << "\n";
    // std::istream response_stream(&response_);
    std::string http_version;
    unsigned int status_code;
    response_stream >> http_version >> status_code;
    std::cout << "Status Code: " << status_code << std::endl;
    if (status_code == 200)
    {
        boost::asio::read_until(socket, response_, "\r\n\r\n");
        std::string response_body;
        std::ostringstream response_body_stream;
        response_body_stream << &response_;
        response_body = response_body_stream.str();

        // Read the remaining response data as the body
        size_t content_start = response_body.find("\r\n\r\n") + 4; // Find the start of the content
        std::string response_content = response_body.substr(content_start);
        std::cout << "Response Content:\n"
                  << response_content << std::endl;
        rapidjson::Document document;
        document.Parse(response_content.c_str());

        if (!document.HasParseError())
        {

            // std::map<std::string, rapidjson::Value> symbolData;

            const rapidjson::Value &symbols = document["symbols"];
            if (symbols.IsArray())
            {
                for (rapidjson::SizeType i = 0; i < symbols.Size(); i++)
                {
                    const rapidjson::Value &symbol = symbols[i];

                    std::string symbolName = symbol["symbol"].GetString();

                    rapidjson::Value dataObject(rapidjson::kObjectType);

                    if (symbol.HasMember("status") && symbol["status"].IsString())
                    {
                        std::string status = symbol["status"].GetString();
                        dataObject.AddMember("status", rapidjson::Value(status.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                    }

                    if (symbol.HasMember("contractType") && symbol["contractType"].IsString())
                    {
                        std::string contractType = symbol["contractType"].GetString();
                        dataObject.AddMember("contractType", rapidjson::Value(contractType.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
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
                                        std::string tickSize = filter["tickSize"].GetString();
                                        dataObject.AddMember("tickSize", rapidjson::Value(tickSize.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                                    }
                                }
                                else if (filterType == "LOT_SIZE")
                                {
                                    if (filter.HasMember("stepSize") && filter["stepSize"].IsString())
                                    {
                                        std::string stepSize = filter["stepSize"].GetString();
                                        dataObject.AddMember("stepSize", rapidjson::Value(stepSize.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
                                    }
                                }
                            }
                        }
                    }

                    symbolDataMap[symbolName] = dataObject;
                }
            }

            //  symbolDataMap = std::move(symbolData);

            if (consoleLog)
            {
                consoleLogger->info("Exchange info retrieved successfully");
            }
        }
    }

    else
    {
        if (fileLog)
        {
            fileLogger->error("Request failed with status code: " + std::to_string(status_code));
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->error("Request failed with status code: " + std::to_string(status_code));
            exit(1);
        }
    }
}
void ExchangeInfoClass::getBoostStruct()
{

    boost::asio::io_context io;
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.set_verify_mode(boost::asio::ssl::verify_peer);
    // ctx.load_verify_file("ca.pem");
    ctx.add_verify_path("/etc/ssl/certs");

    ctx.set_options(boost::asio::ssl::context::default_workarounds |
                    boost::asio::ssl::context::no_sslv2 |
                    boost::asio::ssl::context::no_sslv3);
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io,
                                                                  ctx);
    boost::asio::ip::tcp::resolver resolver(io);
    // boost::asio::ip::tcp::socket socket(io);
    boost::system::error_code err;
    boost::asio::ip::tcp::resolver::query query(url, "https");
    std::cout << "1" << std::endl;
    auto const results = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = results;
    // boost::asio::ip::tcp::endpoint connectionEndpoint(endpoint_iterator->address(), 80);
    std::cout << "2" << std::endl;
    boost::asio::ip::tcp::resolver::iterator end;

    boost::asio::connect(socket.lowest_layer(), endpoint_iterator, end, err);
    if (!err)
        std::cout << "connected" << std::endl;
    else
        std::cout << "not connected" << std::endl;
    // boost::asio::connect(socket, endpoint_iterator);

    boost::asio::streambuf request;
    socket.handshake(boost::asio::ssl::stream_base::client);
    {
        std::ostream request_stream(&request);

        request_stream << "GET "
                       << "/fapi/v1/exchangeInfo"
                       << " HTTP/1.1\r\n";
        // request_stream << "GET " << "/fapi/v1/exchangeInfo" << " HTTPS/1.0\r\n";  // note that you can change it if you wish to HTTP/1.0
        request_stream << "Host: " << url << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
    }

    boost::asio::write(socket, request);

    boost::asio::streambuf response_;
    boost::system::error_code ec;
    read(socket, response_, ec);
    std::istream response_stream(&response_);
    // std::cout << &response_ << "\n";
    //  std::cout << &response_ << "\n";
    // std::istream response_stream(&response_);
    std::string http_version;
    unsigned int status_code;
    response_stream >> http_version >> status_code;
    std::cout << "Status Code: " << status_code << std::endl;
    if (status_code == 200)
    {
        boost::asio::read_until(socket, response_, "\r\n\r\n");
        std::string response_body;
        std::ostringstream response_body_stream;
        response_body_stream << &response_;
        response_body = response_body_stream.str();

        // Read the remaining response data as the body
        size_t content_start = response_body.find("\r\n\r\n") + 4; // Find the start of the content
        std::string response_content = response_body.substr(content_start);
        std::cout << "Response Content:\n"
                  << response_content << std::endl;
        rapidjson::Document document;
        document.Parse(response_content.c_str());

        if (!document.HasParseError())
        {

            // std::map<std::string, rapidjson::Value> symbolData;

            const rapidjson::Value &symbols = document["symbols"];
            if (symbols.IsArray())
            {
                for (rapidjson::SizeType i = 0; i < symbols.Size(); i++)
                {
                    const rapidjson::Value &symbol = symbols[i];
                    SymbolData dataObject;
                    dataObject.symbolName = symbol["symbol"].GetString();

                    // Create an instance of SymbolData

                    if (symbol.HasMember("status") && symbol["status"].IsString())
                    {
                        dataObject.status = symbol["status"].GetString();
                    }

                    if (symbol.HasMember("contractType") && symbol["contractType"].IsString())
                    {
                        dataObject.contractType = symbol["contractType"].GetString();
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
                                        dataObject.tickSize = filter["tickSize"].GetString();
                                    }
                                }
                                else if (filterType == "LOT_SIZE")
                                {
                                    if (filter.HasMember("stepSize") && filter["stepSize"].IsString())
                                    {
                                        dataObject.stepSize = filter["stepSize"].GetString();
                                    }
                                }
                            }
                        }
                    }

                    symbolDataList.push_back(dataObject); // Add the SymbolData instance to the vector
                }

                //  symbolDataMap = std::move(symbolData);

                if (consoleLog)
                {
                    consoleLogger->info("Exchange info retrieved successfully");
                }
            }
        }
    }
    else
    {
        if (fileLog)
        {
            fileLogger->error("Request failed with status code: " + std::to_string(status_code));
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->error("Request failed with status code: " + std::to_string(status_code));
            exit(1);
        }
    }
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
                
                //std::map<std::string, rapidjson::Value> symbolData;

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

                        symbolDataMap[symbolName] = dataObject;
                    }
                }

              //  symbolDataMap = std::move(symbolData);

                if (consoleLog) {
                    consoleLogger->info("Exchange info retrieved successfully");
                }
            } 
            else {
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
    std::ifstream inputFile("/CPP-PROJECT/files/queryfile.json");

    if (!inputFile.is_open())
    {
        throw std::runtime_error("Failed to open the file.");
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
        std::ifstream inputFile("/CPP-PROJECT/files/queryfile.json");

        if (!inputFile.is_open())
        {
            throw std::runtime_error("Failed to open the file.");
            if (fileLog)
                fileLogger->error("failed to open query.json file");
            if (consoleLog)
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

void ExchangeInfoClass::queryCheck(const rapidjson::Value &queryContent)
{
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
                    if (fileLog)
                    {
                        fileLogger->info("Request repeated");
                        fileLogger->flush();
                    }
                    if (consoleLog)
                    {
                        consoleLogger->debug("Request repeated");
                    }
                }
                else
                {
                    prevData[id] = instrumentName;
                    // ExchangeInfoClass::getData(instrumentName);
                    ExchangeInfoClass::getDataStruct(instrumentName, queryContent);
                    //  ExchangeInfoClass::getData(instrumentName);
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
    }
}
void ExchangeInfoClass::getDataStruct(const std::string &instrumentName, const rapidjson::Value &queryContent)
{
    auto symbolDataIter = std::find_if(symbolDataList.begin(), symbolDataList.end(), [&](const SymbolData &data)
                                       { return data.symbolName == instrumentName; });

    if (symbolDataIter != symbolDataList.end())
    {
        const SymbolData &symbolData = *symbolDataIter;

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

        // Iterate over the queryContent object and add matching fields from symbolData
        if (queryContent.HasMember("data") && queryContent["data"].IsArray())
        {
            const rapidjson::Value &dataFields = queryContent["data"];
            for (rapidjson::SizeType i = 0; i < dataFields.Size(); i++)
            {
                const std::string &fieldName = dataFields[i].GetString();

                if (fieldName == "status" && !symbolData.status.empty())
                {
                    dataObject.AddMember(rapidjson::StringRef(fieldName.c_str()), rapidjson::Value(symbolData.status.c_str(), allocator).Move(), allocator);
                }
                if (fieldName == "contractType" && !symbolData.contractType.empty())
                {
                    dataObject.AddMember(rapidjson::StringRef(fieldName.c_str()), rapidjson::Value(symbolData.contractType.c_str(), allocator).Move(), allocator);
                }
                if (fieldName == "tickSize" && !symbolData.tickSize.empty())
                {
                    dataObject.AddMember(rapidjson::StringRef(fieldName.c_str()), rapidjson::Value(symbolData.tickSize.c_str(), allocator).Move(), allocator);
                }
                if (fieldName == "stepSize" && !symbolData.stepSize.empty())
                {
                    dataObject.AddMember(rapidjson::StringRef(fieldName.c_str()), rapidjson::Value(symbolData.stepSize.c_str(), allocator).Move(), allocator);
                }
            }
        }

        rapidjson::Value result(rapidjson::kObjectType);
        std::string str = std::to_string(id);
        result.AddMember("ID", rapidjson::Value(str.c_str(), allocator).Move(), allocator);
        result.AddMember("symbol", rapidjson::Value(instrumentName.c_str(), allocator).Move(), allocator);
        result.AddMember("data", dataObject, allocator);

        answersArray.PushBack(result, allocator);

        queryDoc["answers"] = answersArray;

        std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
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
            throw std::runtime_error("CANNOT OPEN FILE");
        }

        rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
        queryDoc.Accept(answersWriter);

        ansFileWrite.close();

        if (consoleLog)
        {
            consoleLogger->debug("Data written to the file successfully");
        }
    }
    else
    {
        throw std::runtime_error("Symbol not found.");
    }
}

void ExchangeInfoClass::getData(const std::string &instrumentName)
{

    if (symbolDataMap.find(instrumentName) != symbolDataMap.end())
    {
        // const rapidjson::Value &symbolData = symbolDataMap[instrumentName]; //rapidjson::Value dataObject(symbolDataMap[instrumentName], allocator);

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

        rapidjson::Value dataObject(symbolDataMap[instrumentName], allocator);

        rapidjson::Value result(rapidjson::kObjectType);
        std::string str = std::to_string(id);
        rapidjson::Value strId(str.c_str(), str.size(), allocator);
        result.AddMember("ID", rapidjson::Value(strId, allocator).Move(), allocator);
        result.AddMember("symbol", rapidjson::Value(instrumentName.c_str(), allocator).Move(), allocator);
        result.AddMember("data", dataObject, allocator); //
        // result.AddMember("data", dataObject, allocator);

        answersArray.PushBack(result, allocator);

        queryDoc["answers"] = answersArray;

        std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
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
            throw std::runtime_error("CANNOT OPEN FILE");
        }

        rapidjson::OStreamWrapper answersStreamWrapper(ansFileWrite);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
        queryDoc.Accept(answersWriter);

        ansFileWrite.close();

        if (consoleLog)
        {
            consoleLogger->debug("Data written in file successfully");
        }
    }
    else
    {
        throw std::runtime_error("Symbol not found.");
    }
}

void ExchangeInfoClass::deleteData(int id, std::string instrumentName)
{

    std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
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

    std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
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

        std::ifstream ansFile("/CPP-PROJECT/files/answers.json");

        if (!ansFile.is_open())
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

        std::ofstream ansFileWrite("/CPP-PROJECT/files/answers.json");
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

        if (fileLog)
        {
            fileLogger->info("data updated in file");
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->info("data updated in file");
        }
    }
    else
    {
        if (fileLog)
        {
            fileLogger->error("INVALID DATA");
            fileLogger->flush();
        }

        if (consoleLog)
        {
            consoleLogger->error("INVALID DATA");
        }
    }
}