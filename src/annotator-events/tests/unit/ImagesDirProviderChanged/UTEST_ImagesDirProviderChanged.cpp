#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/annotator-events/events/ImagesDirProviderChanged.h"

using namespace testing;
using events::events::ImagesDirProviderChanged;

TEST(UTEST_ImagesDirProviderChanged, constructor_stores_the_provider_pointer)
{
  // ImagesPathsDBProvider is an abstract provider interface; verify the
  // pointer is stored verbatim using the null case.
  ImagesDirProviderChanged e{nullptr};

  EXPECT_EQ(e.images_provider, nullptr);
}
