#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "src/app/CommandLineParser.h"

using namespace app;
using namespace testing;

class UTEST_CommandLineParser : public Test
{
 public:
  UTEST_CommandLineParser()
      : parser{std::make_shared<CommandLineParser>()},
        appctx{create_context(argc, argv)}
  {
  }

  std::shared_ptr<ApplicationContext> create_context(int& gargc, char**& gargv)
  {
    return std::make_shared<ApplicationContext>(gargc, gargv);
  }

  void two_args(const char* const secondParam)
  {
    static std::string binaryName{"binaryName"};
    static std::string secondArg;

    static char* customArgv[] = {binaryName.data(), secondArg.data()};

    secondArg = secondParam;

    customArgv[1] = secondArg.data();

    argc = 2;
    argv = customArgv;
  }

  /// @brief Places the given command line parameters after a binary name, so
  /// that a flag carrying data may be given together with its value.
  void given_args(const std::vector<std::string>& params)
  {
    args.clear();
    args.emplace_back("binaryName");
    args.insert(args.end(), params.cbegin(), params.cend());

    argvHolder.clear();

    for (auto& arg : args) {
      argvHolder.emplace_back(arg.data());
    }

    argc = static_cast<int>(argvHolder.size());
    argv = argvHolder.data();
  }

  /// @brief The four parameters of a complete conversion request.
  static std::vector<std::string> conversion_args()
  {
    return {"--convert-from", "yolo4", "--convert-src", "/datasets/in",
            "--convert-to",   "coco",  "--convert-dst", "/datasets/out"};
  }

  int argc{0};
  char** argv{nullptr};

  std::vector<std::string> args;
  std::vector<char*> argvHolder;

  std::shared_ptr<CommandLineParser> parser;
  std::shared_ptr<ApplicationContext> appctx;
};

TEST_F(UTEST_CommandLineParser, no_context_error)
{
  EXPECT_FALSE(parser->parse_args({}));
}

TEST_F(UTEST_CommandLineParser, empty_context)
{
  EXPECT_TRUE(parser->parse_args(appctx));
}

TEST_F(UTEST_CommandLineParser, help_short)
{
  two_args("-h");

  EXPECT_CALL(*appctx, push_error(_)).Times(0);

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_TRUE(appctx->print_help_and_exit);
  EXPECT_FALSE(appctx->print_version_and_exit);
  EXPECT_TRUE(appctx->errors.empty());
}

TEST_F(UTEST_CommandLineParser, help_long)
{
  two_args("--help");

  EXPECT_CALL(*appctx, push_error(_)).Times(0);

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_TRUE(appctx->print_help_and_exit);
  EXPECT_FALSE(appctx->print_version_and_exit);
  EXPECT_TRUE(appctx->errors.empty());
}

TEST_F(UTEST_CommandLineParser, version_short)
{
  two_args("-v");

  EXPECT_CALL(*appctx, push_error(_)).Times(0);

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_FALSE(appctx->print_help_and_exit);
  EXPECT_TRUE(appctx->print_version_and_exit);
  EXPECT_TRUE(appctx->errors.empty());
}

TEST_F(UTEST_CommandLineParser, version_long)
{
  two_args("--version");

  EXPECT_CALL(*appctx, push_error(_)).Times(0);

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_FALSE(appctx->print_help_and_exit);
  EXPECT_TRUE(appctx->print_version_and_exit);
  EXPECT_TRUE(appctx->errors.empty());
}

TEST_F(UTEST_CommandLineParser, unknown_flag)
{
  static constexpr const char* const unknownFlag = "--unknown";
  static std::string unknownFlagStr{unknownFlag};
  static const std::string expectedError =
      std::string{"Unknown parameter: "} + unknownFlagStr;

  two_args(unknownFlag);

  EXPECT_CALL(*appctx, push_error(expectedError)).Times(1);

  EXPECT_FALSE(parser->parse_args(appctx));

  EXPECT_TRUE(appctx->print_help_and_exit);
  EXPECT_FALSE(appctx->print_version_and_exit);
  EXPECT_TRUE(appctx->errors.empty());
}

TEST_F(UTEST_CommandLineParser, a_complete_conversion_request)
{
  given_args(conversion_args());

  EXPECT_CALL(*appctx, push_error(_)).Times(0);

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_EQ(appctx->convert_from, "yolo4");
  EXPECT_EQ(appctx->convert_src, "/datasets/in");
  EXPECT_EQ(appctx->convert_to, "coco");
  EXPECT_EQ(appctx->convert_dst, "/datasets/out");

  EXPECT_TRUE(appctx->conversion_requested());

  EXPECT_FALSE(appctx->print_help_and_exit);
  EXPECT_FALSE(appctx->print_version_and_exit);
}

TEST_F(UTEST_CommandLineParser, the_conversion_values_are_no_flags_of_their_own)
{
  given_args({"--convert-from", "plain-txt", "--convert-src", "-h",
              "--convert-to", "coco", "--convert-dst", "/datasets/out"});

  EXPECT_CALL(*appctx, push_error(_)).Times(0);

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_EQ(appctx->convert_src, "-h");
  EXPECT_FALSE(appctx->print_help_and_exit);
}

TEST_F(UTEST_CommandLineParser, a_partial_conversion_request_is_rejected)
{
  given_args({"--convert-from", "yolo4", "--convert-src", "/datasets/in"});

  EXPECT_CALL(*appctx, push_error(_)).Times(1);

  EXPECT_FALSE(parser->parse_args(appctx));

  EXPECT_FALSE(appctx->conversion_requested());

  EXPECT_TRUE(appctx->print_help_and_exit);
  EXPECT_FALSE(appctx->print_version_and_exit);
}

TEST_F(UTEST_CommandLineParser, a_conversion_flag_without_its_data_is_rejected)
{
  given_args({"--convert-from"});

  EXPECT_CALL(*appctx, push_error(_)).Times(1);

  EXPECT_FALSE(parser->parse_args(appctx));

  EXPECT_TRUE(appctx->print_help_and_exit);
  EXPECT_FALSE(appctx->print_version_and_exit);
}

TEST_F(UTEST_CommandLineParser, no_conversion_flag_is_no_conversion_request)
{
  given_args({"--version"});

  EXPECT_TRUE(parser->parse_args(appctx));

  EXPECT_FALSE(appctx->conversion_requested());
}
