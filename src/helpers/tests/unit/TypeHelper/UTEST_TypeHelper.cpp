#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/helpers/TypeHelper.h"

using namespace testing;
using helpers::TypeHelper;

TEST(UTEST_TypeHelper, toD_casts_the_value_to_double)
{
  EXPECT_DOUBLE_EQ(TypeHelper::toD(5), 5.0);
  EXPECT_DOUBLE_EQ(TypeHelper::toD(-3), -3.0);
  EXPECT_DOUBLE_EQ(TypeHelper::toD(2.5f), 2.5);
}

TEST(UTEST_TypeHelper, toI_truncates_towards_zero)
{
  EXPECT_EQ(TypeHelper::toI(3.9), 3);
  EXPECT_EQ(TypeHelper::toI(-2.9), -2);
  EXPECT_EQ(TypeHelper::toI(7.0), 7);
}

TEST(UTEST_TypeHelper, ceilInt_rounds_up_to_the_next_integer)
{
  EXPECT_EQ(TypeHelper::ceilInt(3.1), 4);
  EXPECT_EQ(TypeHelper::ceilInt(3.0), 3);
  EXPECT_EQ(TypeHelper::ceilInt(-3.1), -3);
}

TEST(UTEST_TypeHelper, to_string_with_precision_formats_fixed_decimals)
{
  TypeHelper helper;

  EXPECT_EQ(helper.to_string_with_precision(3.14159, 2), "3.14");
  EXPECT_EQ(helper.to_string_with_precision(2.0, 3), "2.000");
  EXPECT_EQ(helper.to_string_with_precision(0, 0), "0");
}
