#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>


int main()
{

    web::http::client::http_client client(U("https://fapi.binance.com"));

    web::uri_builder builder(U("/fapi/v1/exchangeInfo"));

    pplx::task<web::http::http_response> response = client.request(web::http::methods::GET, builder.to_string());
    std::cout << "before wait: " << std::endl;

    response.wait();
    std::cout << "after  wait: " << std::endl;

    if (response.get().status_code() == web::http::status_codes::OK)
    {

        pplx::task<web::json::value> jsonBody = response.get().extract_json();

        jsonBody.wait();

        web::json::value exchangeInfo = jsonBody.get();
     //   std::cout << exchangeInfo;
        std::string jsonString = exchangeInfo.serialize();
         rapidjson::Document document;
        document.Parse(jsonString.c_str());
        // std::cout << jsonString << std::endl;
           // std::cout<< document.Parse(jsonString.c_str());
        std::ofstream outputFile("exchangeInfo.json");
        if (outputFile.is_open())
        {
         rapidjson::StringBuffer buffer;
          rapidjson::  Writer<rapidjson::StringBuffer> writer(buffer);
            document.Accept(writer);
            outputFile << buffer.GetString();
            outputFile.close();
        }
        else
        {
            std::cout << "NOT WRITTEN IN FILE." << std::endl;
        }
    }
    else
    {
        std::cout << "FAILURE!!! Status code: " << response.get().status_code() << std::endl;
    }

    return 0;
}
