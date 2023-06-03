#include <benchmark/benchmark.h>
#include "exchangeInfo.h"



static void BM_getData(benchmark::State &state)
{
  ExchangeInfoClass info;
   std::string url="https://fapi.binance.com";
    for (auto _ : state)
    {
       info.getExchangeInfo();
    }
 
}


BENCHMARK(BM_getData);


BENCHMARK_MAIN();