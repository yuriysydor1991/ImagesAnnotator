#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GLOBAL_DECLS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GLOBAL_DECLS_H

#include <string>

namespace project_decls
{

/// @brief Global project' binary name
const std::string PROJECT_NAME = "ImagesAnnotator";
/// @brief Global project' build version
const std::string PROJECT_BUILD_VERSION = "0.11.0";
/// @brief Global project' configure git commit hash
const std::string PROJECT_BUILD_COMMIT =
    "88deeddd1cf81e59c4e07a8d33d73bbb88cb5bc7";
/// @brief Global project' CMake configure date
const std::string PROJECT_CONFIGURE_DATE = "2025-07-08 16:31:04";
/// @brief Global project' flatpak App URL
const std::string PROJECT_FLATPAK_URL =
    "ua.org.kytok.template.gtkmm3.ImagesAnnotator";
const std::string PROJECT_MAINTAINER = "Yuriy Sydor";
const std::string PROJECT_MAINTAINER_EMAIL = "yuriysydor1991@gmail.com";
const std::string PROJECT_HOMEPAGE_URL = "http://www.kytok.org.ua/";
const std::string PROJECT_DESCRIPTION =
    "The data images annotation project with GTKmm3 and Glade usage";

}  // namespace project_decls

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GLOBAL_DECLS_H
