#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/events/DisplayErrorEvent.h"

using namespace testing;
using events::events::DisplayErrorEvent;

TEST(UTEST_DisplayErrorEvent, constructor_stores_the_value)
{
  DisplayErrorEvent e{"boom happened"};

  EXPECT_EQ(e.description, "boom happened");
}

TEST(UTEST_DisplayErrorEvent, create_returns_a_populated_event)
{
  auto e = DisplayErrorEvent::create("boom happened");

  ASSERT_NE(e, nullptr);
  EXPECT_EQ(e->description, "boom happened");
}
