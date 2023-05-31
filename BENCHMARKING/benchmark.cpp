#include <benchmark/benchmark.h>
#include "exchangeInfo.h"



static void BM_getData(benchmark::State &state)
{
  
    for (auto _ : state)
    {
         int N = state.range(0);
        
     // ExchangeInfoClass::getData();
        
    }
 
}



BENCHMARK(BM_getData)->RangeMultiplier(3)->Range(1,1<<10)->Complexity(benchmark::oAuto);


BENCHMARK_MAIN();