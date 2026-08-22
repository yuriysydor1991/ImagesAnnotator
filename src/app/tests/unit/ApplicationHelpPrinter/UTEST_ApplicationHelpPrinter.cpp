#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/app/ApplicationHelpPrinter.h"
#include "src/app/converter/ConversionFormats.h"

using namespace app;
using namespace testing;

class UTEST_ApplicationHelpPrinter : public Test
{
 public:
  UTEST_ApplicationHelpPrinter()
      : app{std::make_shared<ApplicationHelpPrinter>()},
        appCtx{std::make_shared<ApplicationContext>(argc, argv)}
  {
  }

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<ApplicationHelpPrinter> app;
  std::shared_ptr<ApplicationContext> appCtx;
};

TEST_F(UTEST_ApplicationHelpPrinter, no_context_error)
{
  EXPECT_NE(app->run({}), 0);
}

TEST_F(UTEST_ApplicationHelpPrinter, normal_exit)
{
  EXPECT_CALL(*appCtx, push_error(_)).Times(0);

  EXPECT_EQ(app->run(appCtx), 0);

  EXPECT_TRUE(appCtx->errors.empty());

  EXPECT_FALSE(appCtx->print_help_and_exit);
  EXPECT_FALSE(appCtx->print_version_and_exit);
}

TEST_F(UTEST_ApplicationHelpPrinter,
       the_conversion_flags_and_formats_are_printed)
{
  internal::CaptureStdout();

  EXPECT_EQ(app->run(appCtx), 0);

  const std::string printed = internal::GetCapturedStdout();

  EXPECT_THAT(printed, HasSubstr("--convert-from"));
  EXPECT_THAT(printed, HasSubstr("--convert-src"));
  EXPECT_THAT(printed, HasSubstr("--convert-to"));
  EXPECT_THAT(printed, HasSubstr("--convert-dst"));

  for (const auto& name : converter::ConversionFormats::names()) {
    EXPECT_THAT(printed, HasSubstr(name));
  }
}
