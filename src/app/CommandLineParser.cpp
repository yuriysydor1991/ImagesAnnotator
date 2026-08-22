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

#include "src/app/CommandLineParser.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <set>
#include <string>

#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

namespace app
{

namespace
{
const std::string HELPW{"--help"};
const std::string HELP{"-h"};
const std::string VERSIONW{"--version"};
const std::string VERSION{"-v"};
const std::string CONVERT_FROM{"--convert-from"};
const std::string CONVERT_SRC{"--convert-src"};
const std::string CONVERT_TO{"--convert-to"};
const std::string CONVERT_DST{"--convert-dst"};
}  // namespace

bool CommandLineParser::parse_args(std::shared_ptr<ApplicationContext> ctx)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid application context provided");
    return false;
  }

  for (int iter = 1; iter < ctx->argc; ++iter) {
    const int nextIter = iter + 1;
    const bool hasNext = nextIter < ctx->argc;

    const std::string param = ctx->argv[iter];
    const std::string nextParam = hasNext ? ctx->argv[nextIter] : std::string{};

    if (!parse_arg(ctx, param, hasNext, nextParam, iter)) {
      LOGE("Failure to parse arg: " << param);
      return false;
    }
  }

  return check_conversion_request(ctx);
}

bool CommandLineParser::check_conversion_request(
    std::shared_ptr<ApplicationContext> ctx)
{
  const bool anyGiven = !ctx->convert_from.empty() ||
                        !ctx->convert_src.empty() || !ctx->convert_to.empty() ||
                        !ctx->convert_dst.empty();

  if (!anyGiven || ctx->conversion_requested()) {
    return true;
  }

  static const std::string e =
      "A conversion requires all of the " + CONVERT_FROM + ", " + CONVERT_SRC +
      ", " + CONVERT_TO + " and " + CONVERT_DST + " parameters.";

  ctx->print_help_and_exit = true;
  ctx->push_error(e);
  LOGE(e);

  return false;
}

bool CommandLineParser::check_4_data(std::shared_ptr<ApplicationContext> ctx,
                                     const std::string& param,
                                     const int& hasNext,
                                     const std::string& nextParam)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid application context provided");
    return false;
  }

  const bool requiresData = requires_data(param);

  if (requiresData && !hasNext) {
    ctx->print_help_and_exit = true;
    ctx->push_error("Parameter " + param + " requires the data next to it.");
    LOGE("Parameter " << param << " requires the data next to it.");
    return false;
  }

  return (hasNext && !nextParam.empty()) || true;
}

bool CommandLineParser::parse_arg(std::shared_ptr<ApplicationContext> ctx,
                                  const std::string& param, const int& hasNext,
                                  const std::string& nextParam, int& paramIndex)
{
  assert(ctx != nullptr);

  if (!check_4_data(ctx, param, hasNext, nextParam)) {
    LOGE("Failure with param data");
    return false;
  }

  // add a new params parse over here
  // Also register new command line parameters in the ApplicationhelpPrinter's
  // help.
  if (param == HELPW || param == HELP) {
    ctx->print_help_and_exit = true;
  } else if (param == VERSIONW || param == VERSION) {
    ctx->print_version_and_exit = true;
  } else if (param == CONVERT_FROM) {
    ctx->convert_from = nextParam;
  } else if (param == CONVERT_SRC) {
    ctx->convert_src = nextParam;
  } else if (param == CONVERT_TO) {
    ctx->convert_to = nextParam;
  } else if (param == CONVERT_DST) {
    ctx->convert_dst = nextParam;
  } else {
    ctx->print_help_and_exit = true;
    ctx->push_error("Unknown parameter: " + param);
    LOGE("Unknown parameter: " << param);
    return false;
  }

  if (hasNext && requires_data(param)) {
    paramIndex++;
  }

  return (hasNext && nextParam.empty()) || true;
}

const std::set<std::string>& CommandLineParser::get_params_requiring_data()
{
  // Place here command line parameters that are requiring
  // some data after it.
  static const std::set<std::string> requireNext{CONVERT_FROM, CONVERT_SRC,
                                                 CONVERT_TO, CONVERT_DST};

  return requireNext;
}

bool CommandLineParser::requires_data(const std::string& param)
{
  const auto& requireNext = get_params_requiring_data();

  return std::find(requireNext.cbegin(), requireNext.cend(), param) !=
         requireNext.cend();
}

}  // namespace app
