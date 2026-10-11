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

#ifndef IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_CONVERSIONFORMATS_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_CONVERSIONFORMATS_CLASS_H

#include <string>
#include <vector>

/**
 * @brief The namespace of the command line driven annotations format
 * conversion, which runs the very same import and export pipeline the graphical
 * interface drives, with no window opened.
 */
namespace app::converter
{

/**
 * @brief The annotations layouts a conversion may name on either of its sides.
 *
 * Every one of them but the native project database is a dataset layout of the
 * ImagesAnnotator-DataImporters and ImagesAnnotator-DataExporters libraries;
 * the native one is the project JSON file the ImagesAnnotator-DataDrivers
 * library reads and writes. The four darknetxx ones are its training
 * directories of the dxxwjz2 and the dxxwjz3 networks and its dxxwjz2p and
 * dxxwjz3p training projects, the source path of which names the project file
 * or the directory holding it.
 */
enum class ConversionFormat
{
  imagesannotator,
  plain_txt,
  yolo4,
  ultralytics_detect,
  ultralytics_obb,
  ultralytics_segment,
  coco,
  pascal_voc,
  createml,
  pytorch,
  dxxwjz2,
  dxxwjz2p,
  dxxwjz3,
  dxxwjz3p
};

/**
 * @brief The command line spellings of the conversion formats.
 */
class ConversionFormats
{
 public:
  virtual ~ConversionFormats() = default;
  ConversionFormats() = default;

  /**
   * @brief Resolves the command line spelling of an annotations format.
   *
   * @param name The spelling given after a --convert-from or a --convert-to
   * command line flag.
   * @param format Out: the resolved format, untouched for an unknown spelling.
   *
   * @return Returns true when the spelling names a format and false otherwise.
   */
  static bool parse(const std::string& name, ConversionFormat& format);

  /**
   * @brief The accepted spellings, in the order the help message prints them.
   *
   * @return Returns the accepted command line spellings.
   */
  static const std::vector<std::string>& names();
};

}  // namespace app::converter

#endif  // IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_CONVERSIONFORMATS_CLASS_H
