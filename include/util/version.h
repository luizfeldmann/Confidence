#ifndef _VERSION_H_
#define _VERSION_H_

//! @brief Stores the application's version information
class Version
{
public:
    static const unsigned uMajor;           //!< Major version number
    static const unsigned uMinor;           //!< Minor version number
    static const unsigned uPatch;           //!< Patch version number
    static const unsigned uTweak;           //!< Tweak version number
    static const char* const szVersion;     //!< String representation of the version in format "major.minor.patch.tweak"
    static const char* const szTimestamp;   //!< Holds the date and time of the build
};

#endif