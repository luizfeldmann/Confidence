/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "util/version.h"

/* static */ const unsigned     Version::uMajor = (${PROJECT_VERSION_MAJOR});
/* static */ const unsigned     Version::uMinor = (${PROJECT_VERSION_MINOR});
/* static */ const unsigned     Version::uPatch = (${PROJECT_VERSION_PATCH});
/* static */ const unsigned     Version::uTweak = (${PROJECT_VERSION_TWEAK});
/* static */ const char* const  Version::szVersion = "${CMAKE_PROJECT_VERSION}";
/* static */ const char* const  Version::szTimestamp = __DATE__ " " __TIME__;