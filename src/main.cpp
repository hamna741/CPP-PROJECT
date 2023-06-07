#include <iostream>
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
    //  while (true)
    // {
    //     info.getBoostStruct();
    //     std::this_thread::sleep_for(std::chrono::seconds(interval));
    // }
      std::thread reloadThread(reloadData,interval);
    //  helper.detach();
   // helper.join();
    // info.getBoostStruct();
//  auto exec_run = [](){ while (true) info.getBoostStruct(); };
//     std::thread t(exec_run);
//     t.join();
sleep(interval+5);
// std::cout<<"waited";
std::thread queryThread(readQuery);
       queryThread.join();
    // info.getExchangeInfo();

  //   info.readQueryFile();
    //  info.getData();
    // info.foo();

    return 0;
}
