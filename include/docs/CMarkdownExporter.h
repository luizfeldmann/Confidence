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

    //! @copydoc IDocExporter::Finish
    bool Finish() override;

    //! @copydoc IDocExporter::GetType
    EDocExporter GetType() const override;

    //! @brief Creates a new instance of this class
    static IDocExporter* Create();
};

DECLARE_SERIALIZATION_SCHEME(CMarkdownExporter);

#endif