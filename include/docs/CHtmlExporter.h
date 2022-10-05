#ifndef _CHTMLEXPORTER_H_
#define _CHTMLEXPORTER_H_

#include "CStoredOutputFileName.h"
#include "CCommonMarkExporter.h"

//! @copydoc EDocExporter::HTML
class CHtmlExporter : public CCommonMarkExporter
                    , public CStoredOutputFileName
{
public:
    CHtmlExporter();

    //! @copydoc IDocExporter::Finish
    bool Finish() override;

    //! @copydoc IDocExporter::GetType
    EDocExporter GetType() const override;

    //! @brief Creates a new instance of this class
    static IDocExporter* Create();
};

DECLARE_SERIALIZATION_SCHEME(CHtmlExporter);

#endif
