/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONCONTEXT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONCONTEXT_CLASS_H

#include <string>
#include <vector>

#include "project-global-decls.h"
#include "src/annotator-events/IImagesAnnotatorEventController.h"

namespace app
{

/**
 * @brief The class that holds necessary information for the application
 * to run properly.
 *
 * May hold parsed parameters, configuration values and error messages
 * with application status code. The command line parameters are parsed by
 * the CommandLineParser class instance inside the ApplicationFactory class.
 * The ApplicationHelpPrinter class instance is printing command line help
 * for available command line parameters that may be stored in
 * the current class instance (if registered properly).
 */
struct ApplicationContext
{
  /// @brief Command line arguments count passed by the OS through the main
  /// executable function.
  int& argc;

  /// @brief The command line arguments array passed by the OS through
  /// the main executable function.
  char**& argv;

  /**
   * @brief An ApplicationContext constructor.
   *
   * @param gargc Command line arguments count passed by the OS through the main
   * executable function that will be referenced in the argc member field.
   * @param gargv  The command line arguments array passed by the OS through
   * the main executable function that will be referenced in the argv member
   * field.
   */
  ApplicationContext(int& gargc, char**& gargv);

  /**
   * @brief Set up this flag to true so the ApplicationFactory will create
   * a ApplicationHelpPrinter instance during the application execution.
   * All other classes are ignoring this flag.
   */
  bool print_help_and_exit{false};

  /**
   * @brief Set up this flag value to a true in order to ApplicationFactory
   * to create ApplicationVersionPrinter and execute it.
   */
  bool print_version_and_exit{false};

  /**
   * @brief Errors description.
   */
  std::vector<std::string> errors;

  /**
   * @brief Pushes a new error description into the errors field;
   *
   * @param errorDescription The application error description.
   */
  void push_error(const std::string& errorDescription);

  /// @brief the path to the directory that holds annotated images.
  std::string images_db_path{};

  /// @brief The path to the directory which holds all the annotations.
  std::string annotation_db_path{};

  std::shared_ptr<events::IImagesAnnotatorEventController> eventer;

  const std::string& app_dbus_url{project_decls::PROJECT_FLATPAK_URL};
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONCONTEXT_CLASS_H
