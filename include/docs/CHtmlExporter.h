/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CHTMLEXPORTER_H_
#define _CHTMLEXPORTER_H_

#include "CStoredOutputFileName.h"
#include "CCommonMarkExporter.h"

class CMarkCustomBlock;
class CHtmlExporterPrivate;

//! @copydoc EDocExporter::HTML
class CHtmlExporter : public CCommonMarkExporter
                    , public CStoredOutputFileName
{
private:
    //! Stores private data of the exporter
    std::unique_ptr<CHtmlExporterPrivate> m_pPrivate;

    bool CreateHtml();
    bool CreateHead(CMarkCustomBlock&);
    bool CreateStyles(CMarkCustomBlock&);
    bool CreateBody(CMarkCustomBlock&);

public:
    CHtmlExporter();

    //! @copydoc IDocExporter::IncrementHeading
    void IncrementHeading() override;

    //! @copydoc IDocExporter::DecrementHeading
    void DecrementHeading() override;

    //! @copydoc IDocExporter::Container
    bool Container(const std::string& strClass, const std::string& strId) override;

    //! @copydoc IDocExporter::Start
    bool Start(std::ostream& rOutput) override;

    //! @copydoc IDocExporter::Finish
    bool Finish() override;

    //! @copydoc IDocExporter::GetType
    EDocExporter GetType() const override;

    //! @copydoc IDocumentationContext::Dependency
    void Dependency(const std::string& strSrc, const std::string& strDst) override;

    //! @brief Creates a new instance of this class
    static IDocExporter* Create();
};

DECLARE_SERIALIZATION_SCHEME(CHtmlExporter);

#endif
