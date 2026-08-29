#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/app/ApplicationFactory.h"
#include "src/app/applications/Application.h"
#include "src/app/applications/ApplicationHelpPrinter.h"
#include "src/app/applications/ApplicationVersionPrinter.h"
#include "src/app/applications/converter/ApplicationConverterComponent.h"
#include "src/app/applications/converter/ApplicationFormatsConverter.h"

using namespace app;
using namespace app::converter;
using namespace testing;

class UTEST_ApplicationFactory : public Test
{
 public:
  UTEST_ApplicationFactory() : factory{std::make_shared<ApplicationFactory>()}
  {
  }

  ~UTEST_ApplicationFactory()
  {
    CommandLineParser::onMockCreate = nullptr;
    Application::onMockCreate = nullptr;
    ApplicationHelpPrinter::onMockCreate = nullptr;
    ApplicationVersionPrinter::onMockCreate = nullptr;
    ApplicationConverterComponent::onMockCreate = nullptr;
    ApplicationFormatsConverter::onMockCreate = nullptr;
  }

  /// @brief Fills the four parameters a conversion is asked for with.
  static void given_the_conversion_request(
      std::shared_ptr<ApplicationContext> ctx)
  {
    ctx->convert_from = "yolo4";
    ctx->convert_src = "/datasets/in";
    ctx->convert_to = "coco";
    ctx->convert_dst = "/datasets/out";
  }

  inline std::shared_ptr<ApplicationContext> create_context(int& gargc,
                                                            char**& gargv)
  {
    return std::make_shared<ApplicationContext>(gargc, gargv);
  }

  std::shared_ptr<ApplicationFactory> factory;

  int customArgc{0};
  char** customArgv{nullptr};
};

TEST_F(UTEST_ApplicationFactory, create_default_context)
{
  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<ApplicationContext> ctx =
      factory->create_default_context(argc, argv);

  EXPECT_NE(ctx, nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_default_arg_parser)
{
  std::shared_ptr<CommandLineParser> parser =
      factory->create_default_arg_parser();

  EXPECT_NE(parser, nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_default_application)
{
  std::shared_ptr<IApplication> app = factory->create_default_application();

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<Application>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_help_printer)
{
  std::shared_ptr<IApplication> app = factory->create_help_printer();

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationHelpPrinter>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_version_printer)
{
  std::shared_ptr<IApplication> app = factory->create_version_printer();

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationVersionPrinter>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_context)
{
  MockFunction<void(CommandLineParser & instance)> onMockCreateEnsurer;

  EXPECT_CALL(onMockCreateEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](CommandLineParser& instance) {
        EXPECT_CALL(instance, parse_args(_))
            .Times(1)
            .WillOnce(Invoke([&](std::shared_ptr<ApplicationContext> ctx) {
              EXPECT_EQ(&ctx->argc, &customArgc);
              EXPECT_EQ(&ctx->argv, &customArgv);
              return true;
            }));
      }));

  CommandLineParser::onMockCreate = onMockCreateEnsurer.AsStdFunction();

  std::shared_ptr<ApplicationContext> ctx =
      factory->create_context(customArgc, customArgv);

  EXPECT_NE(ctx, nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_application_invalid_context_error)
{
  std::shared_ptr<IApplication> app = factory->create_application({});

  EXPECT_EQ(app, nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_application_default)
{
  std::shared_ptr<ApplicationContext> ctx =
      create_context(customArgc, customArgv);

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<Application>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_application_help_printer)
{
  std::shared_ptr<ApplicationContext> ctx =
      create_context(customArgc, customArgv);

  ctx->print_help_and_exit = true;

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationHelpPrinter>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_application_version_printer)
{
  std::shared_ptr<ApplicationContext> ctx =
      create_context(customArgc, customArgv);

  ctx->print_version_and_exit = true;

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationVersionPrinter>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, factory_run_default_app)
{
  MockFunction<void(CommandLineParser & instance)> onMockCreateParserEnsurer;
  MockFunction<void(Application & instance)> onMockCreateAppEnsurer;

  EXPECT_CALL(onMockCreateParserEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](CommandLineParser& instance) {
        EXPECT_CALL(instance, parse_args(_))
            .Times(1)
            .WillOnce(Invoke([&](std::shared_ptr<ApplicationContext> ctx) {
              EXPECT_EQ(&ctx->argc, &customArgc);
              EXPECT_EQ(&ctx->argv, &customArgv);
              return true;
            }));
      }));

  EXPECT_CALL(onMockCreateAppEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](Application& instance) {
        EXPECT_CALL(instance, run(_)).Times(1).WillOnce(Return(0));
      }));

  CommandLineParser::onMockCreate = onMockCreateParserEnsurer.AsStdFunction();
  Application::onMockCreate = onMockCreateAppEnsurer.AsStdFunction();

  EXPECT_EQ(factory->run(customArgc, customArgv), 0);
}

TEST_F(UTEST_ApplicationFactory, factory_run_help_app)
{
  MockFunction<void(CommandLineParser & instance)> onMockCreateParserEnsurer;
  MockFunction<void(ApplicationHelpPrinter & instance)> onMockCreateAppEnsurer;

  EXPECT_CALL(onMockCreateParserEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](CommandLineParser& instance) {
        EXPECT_CALL(instance, parse_args(_))
            .Times(1)
            .WillOnce(Invoke([&](std::shared_ptr<ApplicationContext> ctx) {
              EXPECT_EQ(&ctx->argc, &customArgc);
              EXPECT_EQ(&ctx->argv, &customArgv);
              ctx->print_help_and_exit = true;
              return true;
            }));
      }));

  EXPECT_CALL(onMockCreateAppEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](ApplicationHelpPrinter& instance) {
        EXPECT_CALL(instance, run(_)).Times(1).WillOnce(Return(0));
      }));

  CommandLineParser::onMockCreate = onMockCreateParserEnsurer.AsStdFunction();
  ApplicationHelpPrinter::onMockCreate = onMockCreateAppEnsurer.AsStdFunction();

  EXPECT_EQ(factory->run(customArgc, customArgv), 0);
}

TEST_F(UTEST_ApplicationFactory, factory_run_version_app)
{
  MockFunction<void(CommandLineParser & instance)> onMockCreateParserEnsurer;
  MockFunction<void(ApplicationVersionPrinter & instance)>
      onMockCreateAppEnsurer;

  EXPECT_CALL(onMockCreateParserEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](CommandLineParser& instance) {
        EXPECT_CALL(instance, parse_args(_))
            .Times(1)
            .WillOnce(Invoke([&](std::shared_ptr<ApplicationContext> ctx) {
              EXPECT_EQ(&ctx->argc, &customArgc);
              EXPECT_EQ(&ctx->argv, &customArgv);
              ctx->print_version_and_exit = true;
              return true;
            }));
      }));

  EXPECT_CALL(onMockCreateAppEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](ApplicationVersionPrinter& instance) {
        EXPECT_CALL(instance, run(_)).Times(1).WillOnce(Return(0));
      }));

  CommandLineParser::onMockCreate = onMockCreateParserEnsurer.AsStdFunction();
  ApplicationVersionPrinter::onMockCreate =
      onMockCreateAppEnsurer.AsStdFunction();

  EXPECT_EQ(factory->run(customArgc, customArgv), 0);
}

TEST_F(UTEST_ApplicationFactory, factory_execute_default_app)
{
  MockFunction<void(CommandLineParser & instance)> onMockCreateParserEnsurer;
  MockFunction<void(Application & instance)> onMockCreateAppEnsurer;

  EXPECT_CALL(onMockCreateParserEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](CommandLineParser& instance) {
        EXPECT_CALL(instance, parse_args(_))
            .Times(1)
            .WillOnce(Invoke([&](std::shared_ptr<ApplicationContext> ctx) {
              EXPECT_EQ(&ctx->argc, &customArgc);
              EXPECT_EQ(&ctx->argv, &customArgv);
              return true;
            }));
      }));

  EXPECT_CALL(onMockCreateAppEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](Application& instance) {
        EXPECT_CALL(instance, run(_)).Times(1).WillOnce(Return(0));
      }));

  CommandLineParser::onMockCreate = onMockCreateParserEnsurer.AsStdFunction();
  Application::onMockCreate = onMockCreateAppEnsurer.AsStdFunction();

  EXPECT_EQ(ApplicationFactory::execute(customArgc, customArgv), 0);
}

TEST_F(UTEST_ApplicationFactory, create_formats_converter)
{
  std::shared_ptr<IFormatsConverter> converter =
      factory->create_formats_converter();

  EXPECT_NE(converter, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationFormatsConverter>(converter),
            nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_formats_converter_application)
{
  std::shared_ptr<IApplication> app =
      factory->create_formats_converter_application();

  auto component =
      std::dynamic_pointer_cast<ApplicationConverterComponent>(app);

  ASSERT_NE(component, nullptr);

  // Both instances come out of the one call: the component is of no use
  // without the converter it was handed.
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationFormatsConverter>(
                component->converter),
            nullptr);
}

TEST_F(UTEST_ApplicationFactory, create_application_formats_converter)
{
  std::shared_ptr<ApplicationContext> ctx =
      create_context(customArgc, customArgv);

  given_the_conversion_request(ctx);

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(app, nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<ApplicationConverterComponent>(app),
            nullptr);
}

TEST_F(UTEST_ApplicationFactory, a_partial_conversion_request_is_no_converter)
{
  std::shared_ptr<ApplicationContext> ctx =
      create_context(customArgc, customArgv);

  ctx->convert_from = "yolo4";

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_EQ(std::dynamic_pointer_cast<ApplicationConverterComponent>(app),
            nullptr);
  EXPECT_NE(std::dynamic_pointer_cast<Application>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, help_wins_over_a_conversion_request)
{
  std::shared_ptr<ApplicationContext> ctx =
      create_context(customArgc, customArgv);

  given_the_conversion_request(ctx);

  ctx->print_help_and_exit = true;

  std::shared_ptr<IApplication> app = factory->create_application(ctx);

  EXPECT_NE(std::dynamic_pointer_cast<ApplicationHelpPrinter>(app), nullptr);
}

TEST_F(UTEST_ApplicationFactory, factory_run_formats_converter_app)
{
  MockFunction<void(CommandLineParser & instance)> onMockCreateParserEnsurer;
  MockFunction<void(ApplicationConverterComponent & instance)>
      onMockCreateAppEnsurer;

  EXPECT_CALL(onMockCreateParserEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](CommandLineParser& instance) {
        EXPECT_CALL(instance, parse_args(_))
            .Times(1)
            .WillOnce(Invoke([&](std::shared_ptr<ApplicationContext> ctx) {
              given_the_conversion_request(ctx);
              return true;
            }));
      }));

  EXPECT_CALL(onMockCreateAppEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](ApplicationConverterComponent& instance) {
        EXPECT_CALL(instance, run(_)).Times(1).WillOnce(Return(0));
      }));

  CommandLineParser::onMockCreate = onMockCreateParserEnsurer.AsStdFunction();
  ApplicationConverterComponent::onMockCreate =
      onMockCreateAppEnsurer.AsStdFunction();

  EXPECT_EQ(factory->run(customArgc, customArgv), 0);
}
