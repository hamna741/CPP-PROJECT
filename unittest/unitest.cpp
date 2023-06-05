#include <gtest/gtest.h>
#include "exchangeInfo.h"
#include <cassert>

TEST(projectTest, getDataTest)
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

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}