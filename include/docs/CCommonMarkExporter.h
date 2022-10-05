#ifndef _CCOMMONMARKEXPORTER_H_
#define _CCOMMONMARKEXPORTER_H_

#include <memory>
#include "IDocExporter.h"

struct cmark_node;
class CCommonMarkExporterSession;

//! @brief Base exporter implementation using CommonMark library as backend
class CCommonMarkExporter : public IDocExporter
{
protected:
    //! Stores the current session of the exporter
    std::unique_ptr<CCommonMarkExporterSession> m_pSession;

    //! Gets a pointer to the root document node of the current session
    struct cmark_node* GetRootNode();

    //! Gets a reference to the output stream associated to the current session 
    std::ostream& GetStream() const;

public:
    CCommonMarkExporter();
    ~CCommonMarkExporter();

    //! @copydoc IDocExporter::Start
    bool Start(std::ostream& rOutput) override;
};

#endif