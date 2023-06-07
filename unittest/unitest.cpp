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
  std::string instrumentName = "ETHUSDT";
  info.deleteData(id, instrumentName);

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

  rapidjson::Document queryObject1, queryObject2;
  queryObject1.SetObject();
  rapidjson::Document::AllocatorType &allocator = queryObject1.GetAllocator();

  queryObject1.AddMember("id", 1287, allocator);
  queryObject1.AddMember("instrument_name", "BCHUSDT", allocator);
  rapidjson::Value newData(rapidjson::kObjectType);
  newData.AddMember("status", "not pending", allocator);
  queryObject1.AddMember("data", newData, allocator);

  queryObject2.AddMember("id", 13, allocator);
  queryObject2.AddMember("instrument_name", "ETHUSDT", allocator);
  rapidjson::Value newData(rapidjson::kObjectType);
  newData.AddMember("status", "not pending", allocator);
  newData.AddMember("ticksize", "0.01", allocator);
  queryObject2.AddMember("data", newData, allocator);

  info.updatetData(queryObject1);

  if (answerDoc.HasMember("answers") && answerDoc["answers"].IsArray())
  {
    rapidjson::Value &answersArray = answerDoc["answers"];
    for (rapidjson::SizeType i = 0; i < answersArray.Size(); i++)
    {
      rapidjson::Value &answerObject = answersArray[i];
      if (answerObject.HasMember("ID") && answerObject.HasMember("symbol") && answerObject["ID"].IsString() && answerObject["symbol"].IsString())
      {
        std::string ansID = answerObject["ID"].GetString();
        std::string ansSymbol = answerObject["symbol"].GetString();

        if (std::stoi(ansID) == 1287 && ansSymbol == "BCHUSDT")
        {

          EXPECT_TRUE(answerObject.HasMember("data") && answerObject["data"].IsObject());
          rapidjson::Value &dataObject = answerObject["data"];
          EXPECT_TRUE(dataObject.HasMember("status") && dataObject["status"].IsString());
          EXPECT_STREQ(dataObject["status"].GetString(), "not pending");
          break;
        }
        if (std::stoi(ansID) == 13 && ansSymbol == "ETHUSDT")
        {

          EXPECT_TRUE(answerObject.HasMember("data") && answerObject["data"].IsObject());
          rapidjson::Value &dataObject = answerObject["data"];
          EXPECT_TRUE(dataObject.HasMember("status") && dataObject["status"].IsString());
          EXPECT_STREQ(dataObject["status"].GetString(), "not pending");
          EXPECT_STREQ(dataObject["ticksize"].GetString(), "0.01");
          break;
        }
      }
    }
  }
}

TEST(projectTest, GetDataTest)
{
  // Prepare the test data
  std::string instrumentName = "BCHUSDT";

  rapidjson::Document queryContent;
  queryContent.SetObject();
  rapidjson::Document::AllocatorType &allocator = queryContent.GetAllocator();
  rapidjson::Value dataFields(rapidjson::kArrayType);
  dataFields.PushBack("status", allocator);
  dataFields.PushBack("contract_type", allocator);
  queryContent.AddMember("data", dataFields, allocator);
  ExchangeInfoClass info;

  info.getBoostStruct();
  info.getDataStruct(instrumentName, queryContent);

  std::ifstream ansFile("/CPP-PROJECT/files/answers.json");
  rapidjson::Document answerDoc;
  rapidjson::IStreamWrapper ansFileStreamWrapper(ansFile);
  answerDoc.ParseStream(ansFileStreamWrapper);
  ansFile.close();

  if (answerDoc.HasMember("answers") && answerDoc["answers"].IsArray())
  {
    rapidjson::Value &answersArray = answerDoc["answers"];
    for (rapidjson::SizeType i = 0; i < answersArray.Size(); i++)
    {
      rapidjson::Value &answerObject = answersArray[i];
      if (answerObject.HasMember("symbol") && answerObject["symbol"].IsString())
      {
        std::string ansSymbol = answerObject["symbol"].GetString();
        if (ansSymbol == instrumentName)
        {

          EXPECT_TRUE(answerObject.HasMember("data") && answerObject["data"].IsObject());
          rapidjson::Value &dataObject = answerObject["data"];

          EXPECT_TRUE(dataObject.HasMember("status") && dataObject["status"].IsString());
          EXPECT_TRUE(dataObject.HasMember("contract_type") && dataObject["contract_type"].IsString());

        
        }
      }
    }
  }

  FAIL() << "No matching entry found in the answers.json file.";
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}