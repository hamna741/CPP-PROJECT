
#include "exchangeInfo.h"

ExchangeInfoClass info;
void reloadData(int interval)
{
    while (true)
    {
        info.getBoostStruct();
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}
void readQuery()
{
   
        info.readQueryFile();
       
}
int main()
{


    int interval = info.configFunc();
  
      std::thread reloadThread(reloadData,interval);///////////////////

sleep(interval+5);                 //////////////////////

std::thread queryThread(readQuery);/////////////////
       queryThread.join();////////////////



    return 0;
}
