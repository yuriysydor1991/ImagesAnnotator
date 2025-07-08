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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONFACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONFACTORY_CLASS_H

#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/app/CommandLineParser.h"
#include "src/app/IApplication.h"

namespace app
{

/**
 * @brief An ApplicationFactory contains main routines to create, configure and
 * start the application in accordance of the given parameters.
 */
class ApplicationFactory
{
 public:
  virtual ~ApplicationFactory() = default;
  ApplicationFactory() = default;

  /**
   * @brief Creates the default context to pass to the application creation
   * routine and/or to the created IApplication's instance run method.
   *
   * @param gargc Count of a given command line parameters.
   * @param gargv An array of a given command line parameters.
   *
   * @return Returns filled application context instance.
   */
  virtual std::shared_ptr<ApplicationContext> create_default_context(
      int& gargc, char**& gargv);

  /**
   * @brief Creates a command line parser instance to parse application context
   * argv data.
   *
   * @return Returns created parser instance.
   */
  virtual std::shared_ptr<CommandLineParser> create_default_arg_parser();

  /**
   * @brief Creates default instance for the application implementation.
   *
   * @return Returns created default application.
   */
  virtual std::shared_ptr<IApplication> create_default_application();

  /**
   * @brief Instantiates a default help message printer.
   *
   * @return Instantiates a default help message printer and returns it.
   */
  virtual std::shared_ptr<IApplication> create_help_printer();

  /**
   * @brief Instantiates a default version message printer.
   *
   * @return Instantiates a default version message printer and returns it.
   */
  virtual std::shared_ptr<IApplication> create_version_printer();

  /**
   * @brief Creates context and perform args parse operation
   *
   * @return Returns filled and parsed application context
   */
  virtual std::shared_ptr<ApplicationContext> create_context(int& gargc,
                                                             char**& gargv);

  /**
   * @brief Creates application in accordance with a given context data.
   *
   * @param ctx A filled ApplicationContext instance to check which application
   * to create.
   *
   * @return Returns created application.
   */
  virtual std::shared_ptr<IApplication> create_application(
      std::shared_ptr<ApplicationContext> ctx);

  /**
   * @brief Creates all necessary instances and calls the appropriate
   * IApplication instance' run method.
   *
   * @param gargc Count of a given command line parameters.
   * @param gargv An array of a given command line parameters.
   *
   * @return Returns application run status. Must return the zero value in case
   * of success and a non zero value in case of any error.
   */
  virtual int run(int& gargc, char**& gargv);

  /**
   * @brief A convenience method to call a run without the factory instance
   * inside the main function.
   *
   * @param gargc Count of a given command line parameters.
   * @param gargv An array of a given command line parameters.
   *
   * @return Returns application run status. See run method.
   */
  static int execute(int& gargc, char**& gargv);
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATIONFACTORY_CLASS_H
