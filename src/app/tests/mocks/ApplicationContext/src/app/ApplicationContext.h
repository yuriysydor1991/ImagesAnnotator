#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONCONTEXT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONCONTEXT_CLASS_H

#include <gmock/gmock.h>

#include <string>
#include <vector>

#include "project-global-decls.h"
#include "src/annotator-events/IImagesAnnotatorEventController.h"

namespace app
{

struct ApplicationContext
{
 public:
  int& argc;
  char**& argv;

  ApplicationContext(int& gargc, char**& gargv);

  bool print_help_and_exit{false};
  bool print_version_and_exit{false};
  std::vector<std::string> errors;

  MOCK_METHOD(void, push_error, (const std::string& errorDescription));

  /// @brief the path to the directory that holds annotated images.
  std::string images_db_path{};

  /// @brief The path to the directory which holds all the annotations.
  std::string annotation_db_path{};

  std::shared_ptr<events::IImagesAnnotatorEventController> eventer;

  const std::string& app_dbus_url{project_decls::PROJECT_FLATPAK_URL};
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONCONTEXT_CLASS_H