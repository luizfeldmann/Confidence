#ifndef _VERSION_H_
#define _VERSION_H_

//! @brief Stores the application's version information
class Version
{
public:
    static const unsigned uMajor;
    static const unsigned uMinor;
    static const unsigned uPatch;
    static const unsigned uTweak;
    static const char* const szVersion;
    static const char* const szTimestamp;
};

#endif