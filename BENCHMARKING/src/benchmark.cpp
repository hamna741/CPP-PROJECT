#include <benchmark/benchmark.h>
#include "exchangeInfo.h"



static void BM_getData(benchmark::State &state)
{
  ExchangeInfoClass info;
   const std::string queryContent = R"({
        "id": 17,
        "query_type": "UPDATE",
        "instrument_name": "BCHUSDT",
        "data": {
            "status": "not pending"
        }
    })";
    rapidjson::Document queryDoc;
    queryDoc.Parse(queryContent.c_str());

    for (auto _ : state) {
        info.updatetData(queryDoc);
    }
 
}


BENCHMARK(BM_getData);


BENCHMARK_MAIN();