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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_COMMANDLINEPARSER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_COMMANDLINEPARSER_CLASS_H

#include <memory>
#include <set>

#include "src/app/ApplicationContext.h"

namespace app
{

/**
 * @brief Class for a default implementation of the command line flags
 * and data parsing from and into the given ApplicationContext instance.
 */
class CommandLineParser
{
 public:
  virtual ~CommandLineParser() = default;
  CommandLineParser() = default;

  /**
   * @brief Parses given argc and argv fields of a given application context.
   * In case of any error tries to store it's description into the ctx context
   * and sets value of the print_help_and_exit field of the ctx into true.
   *
   * @param ctx The application context that needs to be parsed.
   *
   * @return Returns true on success and false in case of any error.
   */
  virtual bool parse_args(std::shared_ptr<ApplicationContext> ctx);

 protected:
  /**
   * @brief Parse single argument with optional data provided next to it.
   *
   * @param ctx The parsed application context.
   * @param param Command line argument (usually starts with - or --, for
   * example, --help)
   * @param hasNext Flag that marks if there is any next value placed after
   * the current command line flag.
   * @param nextParam The value that stands after current command line flag, or
   * empty string if insufficient data.
   * @param paramIndex Int iterator that indicates current item in the argv
   * array ( passed mostly by the OS).
   *
   * @return Returns true on the success and false in case of any error.
   */
  virtual bool parse_arg(std::shared_ptr<ApplicationContext> ctx,
                         const std::string& param, const int& hasNext,
                         const std::string& nextParam, int& paramIndex);

  /**
   * @brief Check if given parameter param must have some data value placed
   * after it in the command line parameters.
   *
   * @param ctx The parsed application context.
   * @param param Command line argument (usually starts with - or --, for
   * example, --help)
   * @param hasNext Flag that marks if there is any next value placed after
   * the current command line flag.
   *
   * @return Returns true if given parameter param should have any value and
   * nextParam contains it or command line flag param does not require any data
   * next to it. And a false values otherwise.
   */
  virtual bool check_4_data(std::shared_ptr<ApplicationContext> ctx,
                            const std::string& param, const int& hasNext,
                            const std::string& nextParam);

  /**
   * @brief Method should return the set of command line parameters that are
   * requiring data given next to it.
   */
  virtual const std::set<std::string>& get_params_requiring_data();

  /**
   * @brief Should return a true value if given command line parameter name
   * should have a data next to it and false otherwise.
   */
  virtual bool requires_data(const std::string& param);
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_COMMANDLINEPARSER_CLASS_H
