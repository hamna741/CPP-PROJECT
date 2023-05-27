#include "exchangeInfo.h"

exchangeInfo::exchangeInfoClass::exchangeInfoClass()
{
    id = 0;
    prevID = {0};
    instrumentName = "";
    prevInstrumentName = {""};
}
exchangeInfo::exchangeInfoClass::~exchangeInfoClass()
{
}
int exchangeInfo::exchangeInfoClass::readQueryFile()
{
    std::string prevQueryData;
    std::ifstream inputFile("/home/hamna/Desktop/myproject/CPP-PROJECT/src/queryfile.json");

    if (!inputFile.is_open())
    {
        std::cout << "Failed to open the file." << std::endl;
        return 1;
    }

    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::string currentData(fileSize, ' ');
    inputFile.read(&currentData[0], fileSize);
    inputFile.close();

    while (true)
    {
        std::ifstream inputFile("/home/hamna/Desktop/myproject/CPP-PROJECT/src/queryfile.json");

        if (!inputFile.is_open())
        {
            std::cout << "Failed to open the file." << std::endl;
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
            std::string appendedData = newData.substr(prevQueryData.length());
           
            queryCheck(appendedData);
            prevQueryData = newData;
        }

        // Wait for 1 second before checking for updates again
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
void exchangeInfo::exchangeInfoClass::queryCheck(std::string &queryContent){
     rapidjson::Document docRead;
    if (queryContent.front() != '{' || queryContent.back() != '}') {
        // Add missing brackets to create a valid JSON object
        std::string wrappedContent = "{ \"query\": [ {" + queryContent;
        docRead.Parse(wrappedContent.c_str());
    } else {
        docRead.Parse(queryContent.c_str());
    }
     
   std::cout<<queryContent<<std::endl;
   
  //  docRead.Parse(queryContent.c_str());
 if (docRead.HasParseError())
    {
        std::cout << "Failed to parse QUERY JSON." << std::endl;
      //  inputFile.close();
        //return 1;
    }

    if (docRead.HasMember("query") && docRead["query"].IsArray())
    {
        const rapidjson::Value &queryArray = docRead["query"];
        for (rapidjson::SizeType i = 0; i < queryArray.Size(); i++)
        {
            const rapidjson::Value &queryObject = queryArray[i];

            if (queryObject.HasMember("id") && queryObject["id"].IsInt())
            {
                id = queryObject["id"].GetInt();
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
                    int countId;
                    int countIns;
                    countId = std::count(prevID.begin(), prevID.end(), id);
                    countIns = std::count(prevInstrumentName.begin(), prevInstrumentName.end(), instrumentName);
                    if (countId > 0 && countIns > 0)
                    {
                        std::cout << "request repeated" << std::endl;
                    }
                    else
                    {
                        prevID.push_back(id);
                        prevInstrumentName.push_back(instrumentName);
                        exchangeInfoClass::getData(instrumentName);
                    }
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
        .then([](web::http::http_response response)
              {
            if (response.status_code() == web::http::status_codes::OK) {
                // Extract and process the response body
                return response.extract_string();
            } else {
                throw std::runtime_error("Request failed with status code: " + std::to_string(response.status_code()));
            } })
        .then([](std::string body)
              {
            
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
            } })
        .then([](pplx::task<void> previousTask)
              {
            try {
                
                previousTask.get();
            } catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << std::endl;
            } })
        .wait();
}

void exchangeInfo::exchangeInfoClass::getData(std::string instrumentName)
{

    
    std::cout << "QUERY TYPE IS GET" << std::endl;

    std::ifstream exchangeInfoFile("/home/hamna/Desktop/myproject/CPP-PROJECT/src/exchangeInfo.json");
    if (!exchangeInfoFile.is_open())
    {
        std::cout << "Failed to open the exchangeInfo.json file." << std::endl;
        exit;
    }

    rapidjson::Document exchangeInfoDoc;
    rapidjson::IStreamWrapper exchangeInfoStreamWrapper(exchangeInfoFile);
    exchangeInfoDoc.ParseStream(exchangeInfoStreamWrapper);

    exchangeInfoFile.close();

    if (exchangeInfoDoc.HasParseError())
    {
        std::cout << "Failed to parse JSON response." << std::endl;
        exit;
    }

    rapidjson::Document queryDoc(rapidjson::kObjectType); // to store query result
    rapidjson::Document::AllocatorType &allocator = queryDoc.GetAllocator();
    rapidjson::Value answers(rapidjson::kArrayType);
    

    // rapidjson::Value dataObject(rapidjson::kObjectType); //to store extracted data
   
    if (exchangeInfoDoc.HasMember("symbols") && exchangeInfoDoc["symbols"].IsArray())
    {
        const rapidjson::Value &symbolsArray = exchangeInfoDoc["symbols"];
         
        for (rapidjson::SizeType i = 0; i < symbolsArray.Size(); i++)
        {
            const rapidjson::Value &symbolObject = symbolsArray[i];

            if (symbolObject.HasMember("symbol") && symbolObject["symbol"].IsString())
            {
                std::string symbol = symbolObject["symbol"].GetString();
                // if (prevID != id)
                // {
                // prevID=id;
                if (symbol == instrumentName)
                {
                    rapidjson::Value dataObject(rapidjson::kObjectType); // to store extracted data
                    std::cout << "Symbol: " << symbol << std::endl;
                    if (symbolObject.HasMember("status") && symbolObject["status"].IsString())
                    {
                        status = symbolObject["status"].GetString();
                        std::cout << "Status: " << status << std::endl;
                        dataObject.AddMember("status", rapidjson::Value(status.c_str(), allocator).Move(), allocator);
                    }

                    if (symbolObject.HasMember("contractType") && symbolObject["contractType"].IsString())
                    {
                        contractType = symbolObject["contractType"].GetString();
                        std::cout << "Contract Type: " << contractType << std::endl;
                        dataObject.AddMember("contract Type", rapidjson::Value(contractType.c_str(), allocator).Move(), allocator);
                    }
                    if (symbolObject.HasMember("filters") && symbolObject["filters"].IsArray())
                    {
                        const rapidjson::Value &filtersArray = symbolObject["filters"];
                        for (rapidjson::SizeType i = 0; i < filtersArray.Size(); i++)
                        {
                            const rapidjson::Value &filterObject = filtersArray[i];

                            if (filterObject.HasMember("filterType") && filterObject["filterType"].IsString())
                            {
                                filterType = filterObject["filterType"].GetString();
                                // std::cout << "Filter Type: " << filterType << std::endl;

                                if (filterType == "PRICE_FILTER")
                                {
                                    if (filterObject.HasMember("tickSize") && filterObject["tickSize"].IsString())
                                    {
                                        tickSize = filterObject["tickSize"].GetString();
                                        std::cout << "Price Filter Tick Size: " << tickSize << std::endl;
                                        dataObject.AddMember("Tick Size:", rapidjson::Value(tickSize.c_str(), allocator).Move(), allocator);
                                    }
                                }
                                else if (filterType == "LOT_SIZE")
                                {
                                    if (filterObject.HasMember("stepSize") && filterObject["stepSize"].IsString())
                                    {
                                        stepSize = filterObject["stepSize"].GetString();
                                        std::cout << "Lot Size Step Size: " << stepSize << std::endl;
                                        dataObject.AddMember("step Size:", rapidjson::Value(stepSize.c_str(), allocator).Move(), allocator);
                                    }
                                }
                            }
                        }
                    }
                    rapidjson::Value result(rapidjson::kObjectType);

                    result.AddMember("symbol", rapidjson::Value(symbol.c_str(), allocator).Move(), allocator);
                    result.AddMember("data", dataObject.Move(), allocator);
                   // queryDoc["answers"].PushBack(result,allocator);
                    answers.PushBack(result, allocator);
                
                    break;

                }
                // }
            }
        }
    }


   // queryDoc.PushBack(answers,allocator);
  //  rapidjson::Value result(rapidjson::kObjectType);
   // queryDoc.AddMember("answers", answers.Move(), allocator);
   queryDoc.AddMember(rapidjson::Value("answers", allocator).Move(), answers.Move(), allocator);
//   if (answers.Size() > 0)
//     {
//         if (queryDoc.HasMember("answers"))
//         {
//             rapidjson::Value &existingAnswers = queryDoc["answers"];
//             if (existingAnswers.IsArray())
//             {
//                 for (rapidjson::SizeType i = 0; i < answers.Size(); i++)
//                 {
//                     existingAnswers.PushBack(answers[i], allocator);
//                 }
//             }
//         }
//         else
//         {
//             queryDoc.AddMember("answers", answers.Move(), allocator);
//         }
//     }

    std::ofstream ansFile("answers.json", std::ios::app);
    if (!ansFile.is_open())
    {
        std::cout << "cant open anser.json file" << std::endl;
        exit;
    }
    rapidjson::OStreamWrapper answersStreamWrapper(ansFile);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> answersWriter(answersStreamWrapper);
    queryDoc.Accept(answersWriter);

    ansFile.close();

    std::cout << "Data written to answers.json file." << std::endl;

}

void exchangeInfo::exchangeInfoClass::deleteData()
{
    std::cout << "QUERY TYPE IS DELETE" << std::endl;
}
void exchangeInfo::exchangeInfoClass::updatetData()
{
    std::cout << "QUERY TYPE IS UPDATE" << std::endl;
}

