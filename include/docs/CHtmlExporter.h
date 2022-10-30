#ifndef _CHTMLEXPORTER_H_
#define _CHTMLEXPORTER_H_

#include "CStoredOutputFileName.h"
#include "CCommonMarkExporter.h"

class CMarkCustomBlock;

//! @copydoc EDocExporter::HTML
class CHtmlExporter : public CCommonMarkExporter
                    , public CStoredOutputFileName
{
private:
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
    bool Container() override;

    //! @copydoc IDocExporter::Start
    bool Start(std::ostream& rOutput) override;

    //! @copydoc IDocExporter::Finish
    bool Finish() override;

    //! @copydoc IDocExporter::GetType
    EDocExporter GetType() const override;

    //! @brief Creates a new instance of this class
    static IDocExporter* Create();
};

DECLARE_SERIALIZATION_SCHEME(CHtmlExporter);

#endif
