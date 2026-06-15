cmake_minimum_required(VERSION 3.13)

include(template-project-dockerers)
# NOTE: nlohmann_json and libcurl are enabled by the pre-src feature-enabler
# for ImagesAnnotator (see template-project-feature-enabler.cmake), so they are
# intentionally not repeated here.
include(template-project-libxml2-enabler)
include(template-project-OpenCV-enabler)

# compression
include(template-project-zlib-enabler)

# images
include(template-project-libpng-enabler)
include(template-project-libjpeg-enabler)
include(template-project-libwebp-enabler)
include(template-project-lunasvg-enabler)
include(template-project-giflib-enabler)
include(template-project-libtiff-enabler)
include(template-project-openexr-enabler)
include(template-project-openjpeg-enabler)
include(template-project-libavif-enabler)
include(template-project-libheif-enabler)
