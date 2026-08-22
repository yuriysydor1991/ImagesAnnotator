#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/app/ApplicationFactory.h"
#include "src/app/ApplicationHelpPrinter.h"
#include "src/app/converter/ApplicationConverterComponent.h"
#include "src/app/converter/ApplicationFormatsConverter.h"

using namespace app;
using namespace app::converter;
using namespace testing;

/**
 * @brief Drives the application factory the way the converter only binary
 * compiles it.
 *
 * The one and only difference between the two binaries of this project is the
 * IMAGES_ANNOTATOR_CONVERTER_ONLY definition this suite is compiled with: the
 * app::Application class is not there, and this factory is what has to know it.
 * That the factory compiles at all here is half of what is checked - this suite
 * offers no stand-in for that class, so an ApplicationFactory.cpp which still
 * named it would not build - and what the factory hands out instead of it is
 * the other half.
 */
class UTEST_ConverterOnlyApplicationFactory : public Test
{
 public:
  UTEST_ConverterOnlyApplicationFactory()
      : factory{std::make_shared<ApplicationFactory>()},
        ctx{std::make_shared<ApplicationContext>(argc, argv)}
  {
  }

  void given_the_conversion_request()
  {
    ctx->convert_from = "yolo4";
    ctx->convert_src = "/datasets/in";
    ctx->convert_to = "coco";
    ctx->convert_dst = "/datasets/out";
  }

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<ApplicationFactory> factory;
  std::shared_ptr<ApplicationContext> ctx;
};

TEST_F(UTEST_ConverterOnlyApplicationFactory, there_is_no_default_application)
{
  std::shared_ptr<IApplication> app = factory->create_default_application();

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationHelpPrinter>(app), nullptr);
}

TEST_F(UTEST_ConverterOnlyApplicationFactory,
       a_command_line_without_a_conversion_gets_the_help)
{
  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(std::dynamic_pointer_cast<ApplicationHelpPrinter>(app), nullptr);
}

TEST_F(UTEST_ConverterOnlyApplicationFactory, a_conversion_request_is_honoured)
{
  given_the_conversion_request();

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(std::dynamic_pointer_cast<ApplicationConverterComponent>(app),
            nullptr);
}

TEST_F(UTEST_ConverterOnlyApplicationFactory, create_formats_converter)
{
  std::shared_ptr<IFormatsConverter> converter =
      factory->create_formats_converter();

  EXPECT_NE(std::dynamic_pointer_cast<ApplicationFormatsConverter>(converter),
            nullptr);
}
