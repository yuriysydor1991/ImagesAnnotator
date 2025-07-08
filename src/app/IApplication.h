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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_IAPPLICATION_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_IAPPLICATION_CLASS_H

#include <memory>

#include "src/app/ApplicationContext.h"

/**
 * @brief The namespace to contain all the entities for
 * the application general purpose classes.
 */
namespace app
{

/**
 * @brief Class interface to be implemented by all available applications.
 * For example the main binary application which contains the main
 * implementation or application to print help message etc.
 */
class IApplication
{
 public:
  virtual ~IApplication() = default;
  IApplication() = default;

  /**
   * @brief the main application routine to execute the implemented application.
   *
   * @param ctx A filled context to pass data to the app instance.
   *
   * @return Implementors should return a zero value in case of success and
   * non-zero value in case of any error.
   */
  virtual int run(std::shared_ptr<ApplicationContext> ctx) = 0;

  /// @brief Commonly used return status in case of any app error.
  inline static constexpr const int INVALID = 1;
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_IAPPLICATION_CLASS_H
