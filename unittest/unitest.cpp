#include <gtest/gtest.h>
#include "exchangeInfo.h"
#include <cassert>

TEST(projectTest, DeleteTest)
{
  ExchangeInfoClass info;
  std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
  rapidjson::Document answerDoc;
  rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
  answerDoc.ParseStream(ansFileStreamWrapper);

  ansFile.close();
  int id = 17;
  int id2 = 99;
  std::string instrumentName = "ETHUSDT";
  info.deleteData(id, instrumentName);
  info.deleteData(id2, instrumentName);

  ASSERT_TRUE(answerDoc.HasMember("answers"));
  ASSERT_TRUE(answerDoc["answers"].IsArray());
  for (rapidjson::SizeType index = 0; index < answerDoc["answers"].Size(); index++)
  {
    ASSERT_TRUE(answerDoc["answers"][index].IsObject());
    ASSERT_TRUE(answerDoc["answers"][index].HasMember("ID"));
    ASSERT_TRUE(answerDoc["answers"][index].HasMember("symbol"));

    std::string ansID = answerDoc["answers"][index]["ID"].GetString();
    std::string ansSymbol = answerDoc["answers"][index]["symbol"].GetString();

    ASSERT_FALSE(ansID == "17" && ansSymbol == instrumentName);
    ASSERT_FALSE(ansID == "90" && ansSymbol == instrumentName); // id doesnt exists in data
  }
}

TEST(projectTest, updateTest)
{
  ExchangeInfoClass info;
  std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
  rapidjson::Document answerDoc;
  rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
  answerDoc.ParseStream(ansFileStreamWrapper);

  ansFile.close();

  rapidjson::Value queryObject1(rapidjson::kObjectType);

  rapidjson::Document d1;

  rapidjson::Document::AllocatorType &allocator = d1.GetAllocator();
  d1.SetObject();

  queryObject1.AddMember("id", 30, allocator);
  queryObject1.AddMember("instrument_name", "BCHUSDT", allocator);
  rapidjson::Value newData(rapidjson::kObjectType);
  newData.AddMember("status", "bkawas", allocator);
  queryObject1.AddMember("data", newData, allocator);

  info.configFunc();
  info.updatetData(queryObject1);
  //  info.updatetData(queryObject2);
  if (answerDoc.HasMember("answers") && answerDoc["answers"].IsArray())
  {

    for (rapidjson::SizeType index = 0; index < answerDoc["answers"].Size(); index++)
    {
      rapidjson::Value &answersArray = answerDoc["answers"][index];
      ASSERT_TRUE(answersArray.IsObject());
      ASSERT_TRUE(answersArray.HasMember("ID"));
      ASSERT_TRUE(answersArray.HasMember("symbol"));
      ASSERT_TRUE(answersArray.HasMember("data"));
      rapidjson::Value &dataObject = answersArray["data"];
      const char *idValue = answersArray["ID"].GetString();
      int queryId = 30;
      if (std::atoi(idValue) == queryId)
      {
        ASSERT_TRUE(dataObject.HasMember("status"));
        ASSERT_TRUE(dataObject["status"].IsString());
        // std::cout<<"id"<<answersArray["ID"].GetString();
        // std::cout<<"sym"<<answersArray["symbol"].GetString();
        // std::cout<<"stat"<<dataObject["status"].GetString();
        EXPECT_STREQ("bkawas", dataObject["status"].GetString());
        break;
      }
    }
  }
}

TEST(projectTest, GetDataTest)
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
  info.getData(instrumentName, queryObject1);

  std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
  rapidjson::Document answerDoc;
  rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
  answerDoc.ParseStream(ansFileStreamWrapper);
  ansFile.close();

  ASSERT_TRUE(answerDoc.HasMember("answers"));
  ASSERT_TRUE(answerDoc["answers"].IsArray());
  // for (rapidjson::SizeType index = 0; index < answerDoc["answers"].Size(); index++)
  // {
  std::cout << "size= " << answerDoc["answers"].Size();
  ASSERT_TRUE(answerDoc["answers"][(answerDoc["answers"].Size()) - 1].IsObject()) << "not object";
  ASSERT_TRUE(answerDoc["answers"][(answerDoc["answers"].Size()) - 1].HasMember("ID")) << "no id";
  ASSERT_TRUE(answerDoc["answers"][(answerDoc["answers"].Size()) - 1].HasMember("symbol")) << "no symbol";

  std::string ansID = answerDoc["answers"][(answerDoc["answers"].Size()) - 1]["ID"].GetString();
  std::string ansSymbol = answerDoc["answers"][(answerDoc["answers"].Size()) - 1]["symbol"].GetString();
  // std::cout<<ansID;
  EXPECT_EQ(ansID, "67") << "ID  = " << ansID;
  // }
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
