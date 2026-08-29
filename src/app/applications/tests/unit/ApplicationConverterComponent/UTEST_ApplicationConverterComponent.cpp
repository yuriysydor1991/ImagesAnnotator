#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/annotator-business/AnnotatorController.h"
#include "src/app/ApplicationContext.h"
#include "src/app/applications/converter/ApplicationConverterComponent.h"
#include "src/app/applications/converter/IFormatsConverter.h"

using namespace app;
using namespace app::converter;
using namespace iannotator;
using namespace testing;

namespace
{

class MockFormatsConverter : public IFormatsConverter
{
 public:
  MOCK_METHOD(bool, convert, (std::shared_ptr<ApplicationContext> ctx),
              (override));
};

}  // namespace

/**
 * @brief Drives the application component of a requested conversion.
 *
 * It is the app::Application of the conversion flags with the converter in
 * place of the graphical interface, and this suite checks that very
 * substitution: the annotations pipeline is brought up before the converter
 * runs, the converter is handed the context the component was run with, and
 * both halves of the pipeline are shut down afterwards. That no graphical
 * interface is started is what this suite is built out of - it links no GUI
 * toolkit and no gui::GUIIniter mock at all, which the component would not
 * compile without had it created one.
 */
class UTEST_ApplicationConverterComponent : public Test
{
 public:
  UTEST_ApplicationConverterComponent()
      : converter{std::make_shared<NiceMock<MockFormatsConverter>>()},
        component{std::make_shared<ApplicationConverterComponent>(converter)},
        appCtx{std::make_shared<ApplicationContext>(argc, argv)}
  {
  }

  ~UTEST_ApplicationConverterComponent() override
  {
    AnnotatorController::onMockCreate = nullptr;
  }

  /// @brief Expects the business controller to come up and to be shut down.
  void given_the_annotations_pipeline(bool inited)
  {
    abcEnsurer = std::make_shared<MockFunction<void(AnnotatorController&)>>();

    EXPECT_CALL(*abcEnsurer, Call(_))
        .Times(1)
        .WillOnce(Invoke([this, inited](AnnotatorController& instance) {
          EXPECT_CALL(instance, init(appCtx)).Times(1).WillOnce(Return(inited));
          EXPECT_CALL(instance, deinit).Times(inited ? 1 : 0);
        }));

    AnnotatorController::onMockCreate = abcEnsurer->AsStdFunction();
  }

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<NiceMock<MockFormatsConverter>> converter;
  std::shared_ptr<ApplicationConverterComponent> component;
  std::shared_ptr<ApplicationContext> appCtx;
  std::shared_ptr<MockFunction<void(AnnotatorController&)>> abcEnsurer;
};

TEST_F(UTEST_ApplicationConverterComponent, no_context_error)
{
  EXPECT_CALL(*converter, convert(_)).Times(0);

  EXPECT_NE(component->run({}), 0);
}

TEST_F(UTEST_ApplicationConverterComponent, no_converter_error)
{
  auto orphan =
      std::make_shared<ApplicationConverterComponent>(IFormatsConverterPtr{});

  EXPECT_NE(orphan->run(appCtx), 0);
}

TEST_F(UTEST_ApplicationConverterComponent, normal_exit)
{
  given_the_annotations_pipeline(true);

  EXPECT_CALL(*converter, convert(appCtx))
      .Times(1)
      .WillOnce(Invoke([this](std::shared_ptr<ApplicationContext> ctx) {
        // The pipeline the conversion is submitted through has to be up by the
        // time the converter is handed the context.
        EXPECT_NE(ctx->eventer, nullptr);
        return true;
      }));

  EXPECT_EQ(component->run(appCtx), 0);

  EXPECT_TRUE(appCtx->errors.empty());
}

TEST_F(UTEST_ApplicationConverterComponent, invalid_business_init_exit_status)
{
  given_the_annotations_pipeline(false);

  EXPECT_CALL(*converter, convert(_)).Times(0);

  EXPECT_NE(component->run(appCtx), 0);
}

TEST_F(UTEST_ApplicationConverterComponent, failing_conversion_exit_status)
{
  given_the_annotations_pipeline(true);

  EXPECT_CALL(*converter, convert(appCtx)).Times(1).WillOnce(Return(false));

  EXPECT_NE(component->run(appCtx), 0);
}
