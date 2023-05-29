#include <iostream>
#include "exchangeInfo.h"


int main()
{  
 
 //info.extractSymbols(jsonString);

  /////////////////////////////////////////////////
  
  ExchangeInfoClass info;
  info.configFunc();
info.getExchangeInfo();
   info.readQueryFile();
   //  info.getExchangeInfo();
   std::cout<<"dfs";
   // info.readQueryFile();
   // info.getData();
   
/////////////////////////////////////////////////////////////////////////////////////////RETRIEVING EXCHANGE INFO
   
///////////////////////////////////////////////////////////////////////////////////////////////////// READING QUERY FILE

    return 0;
}
