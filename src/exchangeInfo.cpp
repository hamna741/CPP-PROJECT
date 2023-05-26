#include "exchangeInfo.h"

exchangeInfo::exchangeInfoClass::exchangeInfoClass()
{
    id = 0;
}
exchangeInfo::exchangeInfoClass::~exchangeInfoClass()
{
}
int exchangeInfo::exchangeInfoClass::readQueryFile()
{
    std::ifstream inputFile("/home/hamna/Desktop/myproject/CPP-PROJECT/src/queryfile.json");
    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    rapidjson::IStreamWrapper streamWrapper(inputFile);

   
    rapidjson::Document docRead;
    docRead.ParseStream(streamWrapper);

    
    if (docRead.HasParseError())
    {
        std::cerr << "Failed to parse the JSON." << std::endl;
        inputFile.close();
        return 1;
    }
   
    if (docRead.HasMember("query") && docRead["query"].IsArray())
    {
        const rapidjson::Value &queryArray = docRead["query"];
        for (rapidjson::SizeType i = 0; i < queryArray.Size(); i++)
        {
            const rapidjson::Value &queryObject = queryArray[i];

           
            if (queryObject.HasMember("id") && queryObject["id"].IsInt())
            {
                int id = queryObject["id"].GetInt();
                std::cout << "ID: " << id << std::endl;
            }
            
            if (queryObject.HasMember("instrument_name") && queryObject["instrument_name"].IsString())
            {
                 instrumentName = queryObject["instrument_name"].GetString();
                std::cout << "Instrument Name: " << instrumentName << std::endl;
            }

         
            if (queryObject.HasMember("query_type") && queryObject["query_type"].IsString())
            {

                std::string queryType = queryObject["query_type"].GetString();
                std::cout << "Query Type: " << queryType << std::endl;
                if (queryType == "GET")
                {
                    exchangeInfoClass::getData(instrumentName);
                }
                else if (queryType == "UPDATE")
                {
                    exchangeInfoClass::updatetData();
                }
                else
                {
                    exchangeInfoClass::deleteData();
                }
            }

           
            if (queryObject.HasMember("data"))
            {
                const rapidjson::Value &data = queryObject["data"];

                
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

    // Close the input file
    inputFile.close();

    return 0;
}

void exchangeInfo::exchangeInfoClass::getExchangeInfo()
{
    web::http::client::http_client client(U("https://fapi.binance.com"));

    web::uri_builder builder(U("/fapi/v1/exchangeInfo"));

    // pplx::task<web::http::http_response> response = client.request(web::http::methods::GET, builder.to_string());
    // std::cout << "before wait: " << std::endl;

    // response.wait();
    // std::cout << "after  wait: " << std::endl;

    // if (response.get().status_code() == web::http::status_codes::OK)
    // {

    //     pplx::task<web::json::value> jsonBody = response.get().extract_json();

    //     jsonBody.wait();

    //     web::json::value exchangeInfo = jsonBody.get();
    //     //   std::cout << exchangeInfo;
    //     jsonData = exchangeInfo.serialize();
    //     rapidjson::Document document;
    //     document.Parse(jsonData.c_str());
    //     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //     // check for get data.
    //     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //     // std::cout << jsonString << std::endl;
    //     // std::cout<< document.Parse(jsonString.c_str());
    //     std::ofstream outputFile("exchangeInfo.json");
    //     if (outputFile.is_open())
    //     {
    //         rapidjson::StringBuffer buffer;
    //         rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    //         document.Accept(writer);
    //         outputFile << buffer.GetString();
    //         outputFile.close();
    //     }
    //     else
    //     {
    //         std::cout << "NOT WRITTEN IN FILE." << std::endl;
    //     }
    // }
    // else
    // {
    //     std::cout << "FAILURE!!! Status code: " << response.get().status_code() << std::endl;
    // }



    // Send GET request and handle the response
    client.request(web::http::methods::GET, builder.to_string())
        .then([](web::http::http_response response) {
            if (response.status_code() == web::http::status_codes::OK) {
                // Extract and process the response body
                return response.extract_string();
            } else {
                throw std::runtime_error("Request failed with status code: " + std::to_string(response.status_code()));
            }
        })
        .then([](std::string body) {
            
            rapidjson::Document document;
            document.Parse(body.c_str());

            if (!document.HasParseError()) {
               
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                document.Accept(writer);

                std::ofstream file("exchange_info.json");
                if (file.is_open()) {
                    file << buffer.GetString();
                    file.close();
                    std::cout << "written in file" << std::endl;
                } else {
                    throw std::runtime_error("error opening/wiritng to file");
                }
            } else {
                throw std::runtime_error("Failed to parse JSON response.");
            }
        })
        .then([](pplx::task<void> previousTask) {
            try {
                
                previousTask.get();
            } catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << std::endl;
            }
        })
        .wait();

   
}

void exchangeInfo::exchangeInfoClass::getData(std::string instrumentName)
{
    std::cout << "QUERY TYPE IS GET" << std::endl;

    std::ifstream exchangeInfoFile("/home/hamna/Desktop/myproject/CPP-PROJECT/src/exchangeInfo.json");
    if (!exchangeInfoFile.is_open())
    {
        std::cerr << "Failed to open the exchangeInfo.json file." << std::endl;
        return;
    }

    rapidjson::Document exchangeInfoDoc;
    rapidjson::IStreamWrapper exchangeInfoStreamWrapper(exchangeInfoFile);
    exchangeInfoDoc.ParseStream(exchangeInfoStreamWrapper);

    exchangeInfoFile.close();

    if (exchangeInfoDoc.HasParseError())
    {
        std::cerr << "Failed to parse JSON response." << std::endl;
        return;
    }

    if (exchangeInfoDoc.HasMember("symbols") && exchangeInfoDoc["symbols"].IsArray())
    {
        const rapidjson::Value& symbolsArray = exchangeInfoDoc["symbols"];
        for (rapidjson::SizeType i = 0; i < symbolsArray.Size(); i++)
        {
            const rapidjson::Value &symbolObject = symbolsArray[i];

            if (symbolObject.HasMember("symbol") && symbolObject["symbol"].IsString())
            {
                std::string symbol = symbolObject["symbol"].GetString();
                
                if (symbol == instrumentName)
                {
                    std::cout << "Symbol: " << symbol << std::endl;
                    if (symbolObject.HasMember("status") && symbolObject["status"].IsString())
                    {
                        std::string status = symbolObject["status"].GetString();
                        std::cout << "Status: " << status << std::endl;
                    }
                  
                  
                    if (symbolObject.HasMember("contractType") && symbolObject["contractType"].IsString())
                    {
                        std::string contractType = symbolObject["contractType"].GetString();
                        std::cout << "Contract Type: " << contractType << std::endl;
                    }
                    if (symbolObject.HasMember("filters") && symbolObject["filters"].IsArray())
                    {
                        const rapidjson::Value &filtersArray = symbolObject["filters"];
                        for (rapidjson::SizeType i = 0; i < filtersArray.Size(); i++)
                        {
                            const rapidjson::Value &filterObject = filtersArray[i];

                            if (filterObject.HasMember("filterType") && filterObject["filterType"].IsString())
                            {
                                std::string filterType = filterObject["filterType"].GetString();
                                //std::cout << "Filter Type: " << filterType << std::endl;

                                if (filterType == "PRICE_FILTER")
                                {
                                    if (filterObject.HasMember("tickSize") && filterObject["tickSize"].IsString())
                                    {
                                        std::string tickSize = filterObject["tickSize"].GetString();
                                        std::cout << "Price Filter Tick Size: " << tickSize << std::endl;
                                    }
                                }
                                else if (filterType == "LOT_SIZE")
                                {
                                    if (filterObject.HasMember("stepSize") && filterObject["stepSize"].IsString())
                                    {
                                        std::string stepSize = filterObject["stepSize"].GetString();
                                        std::cout << "Lot Size Step Size: " << stepSize << std::endl;
                                    }
                                }
                                
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

void exchangeInfo::exchangeInfoClass::deleteData()
{
    std::cout << "QUERY TYPE IS DELETE" << std::endl;
}
void exchangeInfo::exchangeInfoClass::updatetData()
{
    std::cout << "QUERY TYPE IS UPDATE" << std::endl;
}