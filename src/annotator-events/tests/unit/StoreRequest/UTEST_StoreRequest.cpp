#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/StoreRequest.h"

using namespace testing;
using events::events::StoreRequest;

TEST(UTEST_StoreRequest, constructor_stores_the_value)
{
  StoreRequest e{"/db.json"};

  EXPECT_EQ(e.dbpath, "/db.json");
}
