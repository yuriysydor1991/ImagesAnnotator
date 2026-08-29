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

#include "src/app/applications/ApplicationHelpPrinter.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "project-global-decls.h"
#include "src/app/ApplicationBinaryName.h"
#include "src/app/applications/converter/ConversionFormats.h"
#include "src/log/log.h"

namespace app
{

int ApplicationHelpPrinter::run(std::shared_ptr<ApplicationContext> ctx)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid application context provided");
    return INVALID;
  }

  // Register and implement here command line parameters from the
  // CommandLineParser class.
  std::cout << "Usage:" << std::endl
            << std::endl
            << "\t" << application_binary_name() << " [OPTIONS]" << std::endl
            << std::endl
            << "Introduce a new command line flags by registering them in"
            << std::endl
            << "the ApplicationHelpPrinter and the CommandLineParser classes."
            << std::endl
            << std::endl
            << "Where OPTIONS may be next:" << std::endl
            << "\t--help or -h - print current help message" << std::endl
            << "\t--version or -v - print application version, build git "
               "commit and configure date"
            << std::endl
            << "\t--convert-from FORMAT - the annotations format to read"
            << std::endl
            << "\t--convert-src PATH - the path to read the annotations from"
            << std::endl
            << "\t--convert-to FORMAT - the annotations format to write"
            << std::endl
            << "\t--convert-dst PATH - the path to write the annotations into"
            << std::endl;

  print_conversion_help();

  return 0;
}

void ApplicationHelpPrinter::print_conversion_help()
{
  std::cout << std::endl
            << "The four conversion flags go together and convert the"
            << std::endl
            << "annotations without starting the graphical interface."
            << std::endl
            << "A FORMAT is one of:" << std::endl;

  for (const auto& name : converter::ConversionFormats::names()) {
    std::cout << "\t" << name << std::endl;
  }

  std::cout << std::endl
            << "Writing the pytorch one cuts the rectangles out of the"
            << std::endl
            << "pictures, which needs the ImagesAnnotator-DataExporters"
            << std::endl
            << "library built with its OpenCV image cropper." << std::endl;
}

}  // namespace app
