#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SUBSYSTEM_DECLARATIONS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SUBSYSTEM_DECLARATIONS_H

#include <gmock/gmock.h>

class logMock
{
 public:
  inline static testing::MockFunction<void(const std::string& filepath,
                                           const unsigned short& logLvl,
                                           const bool printMessages)>
      LOG_INIT;
  inline static testing::MockFunction<void()> LOG_INIT_DEFAULTS;

  inline static testing::MockFunction<void(const std::string&)> LOGE;
  inline static testing::MockFunction<void(const std::string&)> LOGI;
  inline static testing::MockFunction<void(const std::string&)> LOGW;
  inline static testing::MockFunction<void(const std::string&)> LOGD;
  inline static testing::MockFunction<void(const std::string&)> LOGT;
};

#define LOG_INIT(filepath, logLvl, printMessages) \
  logMock::LOG_INIT_DEFAULTS.AsStdFunction()(filepath, logLvl, printMessages);
#define LOG_INIT_DEFAULTS() logMock::LOG_INIT_DEFAULTS.AsStdFunction()();

#define LOGE(msg)                                             \
  {                                                           \
    std::stringstream logMessageContainer;                    \
    logMessageContainer << msg;                               \
    logMock::LOGE.AsStdFunction()(logMessageContainer.str()); \
  }

#define LOGI(msg)                                             \
  {                                                           \
    std::stringstream logMessageContainer;                    \
    logMessageContainer << msg;                               \
    logMock::LOGI.AsStdFunction()(logMessageContainer.str()); \
  }

#define LOGW(msg)                                             \
  {                                                           \
    std::stringstream logMessageContainer;                    \
    logMessageContainer << msg;                               \
    logMock::LOGW.AsStdFunction()(logMessageContainer.str()); \
  }

#define LOGD(msg)                                             \
  {                                                           \
    std::stringstream logMessageContainer;                    \
    logMessageContainer << msg;                               \
    logMock::LOGD.AsStdFunction()(logMessageContainer.str()); \
  }

#define LOGT(msg)                                             \
  {                                                           \
    std::stringstream logMessageContainer;                    \
    logMessageContainer << msg;                               \
    logMock::LOGT.AsStdFunction()(logMessageContainer.str()); \
  }

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SUBSYSTEM_DECLARATIONS_H
