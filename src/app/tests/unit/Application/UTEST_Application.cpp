#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/annotator-business/AnnotatorController.h"
#include "src/app/Application.h"
#include "src/gui/GUIIniter.h"

using namespace app;
using namespace gui;
using namespace testing;
using namespace iannotator;

class UTEST_Application : public Test
{
 public:
  UTEST_Application()
      : app{std::make_shared<Application>()},
        appCtx{std::make_shared<ApplicationContext>(argc, argv)}
  {
  }

  ~UTEST_Application()
  {
    GUIIniter::onMockCreate = nullptr;
    AnnotatorController::onMockCreate = nullptr;
  }

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<Application> app;
  std::shared_ptr<ApplicationContext> appCtx;
};

TEST_F(UTEST_Application, no_context_error) { EXPECT_NE(app->run({}), 0); }

TEST_F(UTEST_Application, normal_exit)
{
  MockFunction<void(GUIIniter & instance)> initerEnsurer;
  MockFunction<void(AnnotatorController & instance)> abcEnsurer;

  EXPECT_CALL(initerEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](GUIIniter& instance) {
        EXPECT_CALL(instance, run(appCtx)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(instance, deinit).Times(1);
      }));

  EXPECT_CALL(abcEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](AnnotatorController& instance) {
        EXPECT_CALL(instance, init(appCtx)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(instance, deinit).Times(1);
      }));

  GUIIniter::onMockCreate = initerEnsurer.AsStdFunction();
  AnnotatorController::onMockCreate = abcEnsurer.AsStdFunction();

  EXPECT_CALL(*appCtx, push_error(_)).Times(0);

  EXPECT_EQ(app->run(appCtx), 0);

  EXPECT_TRUE(appCtx->errors.empty());

  EXPECT_FALSE(appCtx->print_help_and_exit);
  EXPECT_FALSE(appCtx->print_version_and_exit);
}

TEST_F(UTEST_Application, invalid_bysiness_init_exit_status)
{
  MockFunction<void(GUIIniter & instance)> initerEnsurer;
  MockFunction<void(AnnotatorController & instance)> abcEnsurer;

  EXPECT_CALL(initerEnsurer, Call(_)).Times(0);

  EXPECT_CALL(abcEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](AnnotatorController& instance) {
        EXPECT_CALL(instance, init(appCtx)).Times(1).WillOnce(Return(false));
        EXPECT_CALL(instance, deinit).Times(0);
      }));

  GUIIniter::onMockCreate = initerEnsurer.AsStdFunction();
  AnnotatorController::onMockCreate = abcEnsurer.AsStdFunction();

  EXPECT_CALL(*appCtx, push_error(_)).Times(0);

  EXPECT_NE(app->run(appCtx), 0);

  EXPECT_TRUE(appCtx->errors.empty());

  EXPECT_FALSE(appCtx->print_help_and_exit);
  EXPECT_FALSE(appCtx->print_version_and_exit);
}

TEST_F(UTEST_Application, invalid_gui_init_exit)
{
  MockFunction<void(GUIIniter & instance)> initerEnsurer;
  MockFunction<void(AnnotatorController & instance)> abcEnsurer;

  EXPECT_CALL(initerEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](GUIIniter& instance) {
        EXPECT_CALL(instance, run(appCtx)).Times(1).WillOnce(Return(false));
        EXPECT_CALL(instance, deinit).Times(1);
      }));

  EXPECT_CALL(abcEnsurer, Call(_))
      .Times(1)
      .WillOnce(Invoke([&](AnnotatorController& instance) {
        EXPECT_CALL(instance, init(appCtx)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(instance, deinit).Times(1);
      }));

  GUIIniter::onMockCreate = initerEnsurer.AsStdFunction();
  AnnotatorController::onMockCreate = abcEnsurer.AsStdFunction();

  EXPECT_CALL(*appCtx, push_error(_)).Times(0);

  EXPECT_NE(app->run(appCtx), 0);
}
