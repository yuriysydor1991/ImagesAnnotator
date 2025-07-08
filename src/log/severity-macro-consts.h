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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SEVERITIES_MACROS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SEVERITIES_MACROS_H

#ifndef MACRO_LVL_ERROR
#define MACRO_LVL_ERROR 1U
#endif  // MACRO_LVL_ERROR

#ifndef MACRO_LVL_WARNING
#define MACRO_LVL_WARNING 2U
#endif  // MACRO_LVL_WARNING

#ifndef MACRO_LVL_INFO
#define MACRO_LVL_INFO 3U
#endif  // MACRO_LVL_INFO

#ifndef MACRO_LVL_DEBUG
#define MACRO_LVL_DEBUG 4U
#endif  // MACRO_LVL_DEBUG

#ifndef MACRO_LVL_TRACE
#define MACRO_LVL_TRACE 5U
#endif  // MACRO_LVL_TRACE

#ifndef MAX_LOG_LEVEL
#define MAX_LOG_LEVEL MACRO_LVL_INFO
#endif  // MAX_LOG_LEVEL

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SEVERITIES_MACROS_H