#include <iostream>
#include "exchangeInfo.h"


int main()
{  
 
 //info.extractSymbols(jsonString);

  /////////////////////////////////////////////////
  
  ExchangeInfoClass info;
  int interval=info.configFunc();
  info.getExchangeInfo();
  //  std::thread exchangeInfoThread([&]() {
  //       while (true) {
  //           info.getExchangeInfo();
  //           std::this_thread::sleep_for(std::chrono::seconds(interval));
  //       }
  //   });


   info.readQueryFile();
   //  info.getExchangeInfo();

   // info.readQueryFile();
   // info.getData();

    return 0;
}
