/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CMARKDOWNEXPORTER_H_
#define _CMARKDOWNEXPORTER_H_

#include "CStoredOutputFileName.h"
#include "CCommonMarkExporter.h"

//! @copydoc EDocExporter::Markdown
class CMarkdownExporter : public CCommonMarkExporter
                        , public CStoredOutputFileName
{
public:
    CMarkdownExporter();

    //! @copydoc IDocExporter::Container
    bool Container(const std::string& strClass, const std::string& strId) override;

    //! @copydoc IDocExporter::Finish
    bool Finish() override;

    //! @copydoc IDocExporter::GetType
    EDocExporter GetType() const override;

    //! @copydoc IDocumentationContext::Dependency
    void Dependency(const std::string& strSrc, const std::string& strDst) override;

    //! @brief Creates a new instance of this class
    static IDocExporter* Create();
};

DECLARE_SERIALIZATION_SCHEME(CMarkdownExporter);

#endif