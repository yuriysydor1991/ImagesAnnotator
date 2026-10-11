#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>

#include "src/app/applications/converter/ConversionFormats.h"

using namespace app::converter;
using namespace testing;

TEST(UTEST_ConversionFormats, every_name_resolves_to_the_format_it_stands_for)
{
  const auto& names = ConversionFormats::names();

  ASSERT_FALSE(names.empty());

  for (std::size_t iter = 0U; iter < names.size(); ++iter) {
    ConversionFormat format{};

    EXPECT_TRUE(ConversionFormats::parse(names[iter], format));
    EXPECT_EQ(static_cast<std::size_t>(format), iter);
  }
}

TEST(UTEST_ConversionFormats, the_native_annotations_project_is_a_format)
{
  ConversionFormat format{};

  EXPECT_TRUE(ConversionFormats::parse("imagesannotator", format));
  EXPECT_EQ(format, ConversionFormat::imagesannotator);
}

TEST(UTEST_ConversionFormats, every_dataset_layout_of_the_libraries_is_a_format)
{
  EXPECT_EQ(ConversionFormats::names().size(),
            static_cast<std::size_t>(ConversionFormat::dxxwjz3p) + 1U);
}

TEST(UTEST_ConversionFormats, an_unknown_name_leaves_the_format_alone)
{
  ConversionFormat format{ConversionFormat::coco};

  EXPECT_FALSE(ConversionFormats::parse("nosuch", format));
  EXPECT_EQ(format, ConversionFormat::coco);
}

TEST(UTEST_ConversionFormats, an_empty_name_is_no_format)
{
  ConversionFormat format{};

  EXPECT_FALSE(ConversionFormats::parse({}, format));
}
