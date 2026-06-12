#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/AnnotationsDirChanged.h"

using namespace testing;
using events::events::AnnotationsDirChanged;

TEST(UTEST_AnnotationsDirChanged, constructor_stores_the_value)
{
  AnnotationsDirChanged e{"/db.json"};

  EXPECT_EQ(e.annotations_dir, "/db.json");
}
