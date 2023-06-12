#include <benchmark/benchmark.h>
#include "exchangeInfo.h"

// static void BM_UpdateData(benchmark::State& state)
// {
//     ExchangeInfoClass info;
    
//     rapidjson::Value queryObject1(rapidjson::kObjectType);
//     rapidjson::Document d1;
//     d1.SetObject();
//     rapidjson::Document::AllocatorType& allocator = d1.GetAllocator();

//     queryObject1.AddMember("id", 1287, allocator);
//     queryObject1.AddMember("instrument_name", "BCHUSDT", allocator);
//     rapidjson::Value newData(rapidjson::kObjectType);
//     newData.AddMember("status", "not pending", allocator);
//     queryObject1.AddMember("data", newData, allocator);

//     const std::string queryContent = R"({
//             "id": 30,
//             "query_type": "UPDATE",
//             "instrument_name": "BCHUSDT",
//             "data": {
//                 "status": "not pending"
//             }
//         })";
//     rapidjson::Document queryDoc;
//     queryDoc.Parse(queryContent.c_str());

//     info.updatetData(queryDoc); // Warm-up to ensure data is initialized

//     for (auto _ : state)
//     {
//         state.PauseTiming();
//         // Reset any state modifications performed during the warm-up
//         info.updatetData(queryDoc);
//         state.ResumeTiming();
//         info.updatetData(queryDoc);
//     }
// }
static void BM_updateData(benchmark::State &state)
{
    ExchangeInfoClass info;
    rapidjson::Value queryObject1(rapidjson::kObjectType);
    rapidjson::Document d1;
    d1.SetObject();
    rapidjson::Document::AllocatorType &allocator = d1.GetAllocator();
    info.configFunc();
    queryObject1.AddMember("id", 1287, allocator);
    queryObject1.AddMember("instrument_name", "BCHUSDT", allocator);
    rapidjson::Value newData(rapidjson::kObjectType);
    newData.AddMember("status", "not pending", allocator);
    queryObject1.AddMember("data", newData, allocator);

    const std::string queryContent = R"({
            "id": 20,
            "query_type": "UPDATE",
            "instrument_name": "BCHUSDT",
            "data": {
                "status": "not pending"
            }
        })";
    rapidjson::Document queryDoc;
    queryDoc.Parse(queryContent.c_str());

    for (auto _ : state)
    {
        info.updatetData(queryObject1);
    }
}

static void BM_DeleteData(benchmark::State &state)
{
    ExchangeInfoClass info;
    int id = 17;
    std::string instrumentName = "BCHUSDT";

    for (auto _ : state)
    {
        info.deleteData(id, instrumentName);
    }
}
static void BM_GetData(benchmark::State &state)
{
     int id = 67;
  ExchangeInfoClass info;
  std::string instrumentName = "BCHUSDT";

  rapidjson::Value queryObject1(rapidjson::kObjectType), queryObject2(rapidjson::kObjectType);
  rapidjson::Document d1, d2;
  d1.SetObject();
  rapidjson::Document::AllocatorType &allocator = d1.GetAllocator();

  queryObject1.AddMember("id", id, allocator);
  queryObject1.AddMember("instrument_name", "BCHUSDT", allocator);
  rapidjson::Value DataArr(rapidjson::kArrayType);
  DataArr.PushBack("status", allocator);
  DataArr.PushBack("ticksize", allocator);
  queryObject1.AddMember("data", DataArr, allocator);

  info.configFunc();
  info.getExchangeInfo();


    for (auto _ : state)
    {
          info.getData(instrumentName, queryObject1);
    }
}

BENCHMARK(BM_updateData);
BENCHMARK(BM_DeleteData);
BENCHMARK(BM_GetData);
BENCHMARK_MAIN();
