/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "util/CTempFile.h"
#include <cstdio>
#include <filesystem>

CTempFile::CTempFile()
    : m_strName(::tmpnam(nullptr))
{

}

CTempFile::~CTempFile()
{
    if (std::filesystem::exists(m_strName))
        std::filesystem::remove(m_strName);
}

CTempFile::CTempFile(CTempFile&& rMove) noexcept
    : m_strName(rMove.m_strName)
{
    rMove.m_strName.clear();
}

CTempFile& CTempFile::operator=(CTempFile&& rMove) noexcept
{
    m_strName = rMove.m_strName;
    rMove.m_strName.clear();

    return *this;
}

std::string CTempFile::path() const
{
    return m_strName;
}