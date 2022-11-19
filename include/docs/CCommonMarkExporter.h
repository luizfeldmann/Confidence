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
    //! Creates a new node of the requested type and (optionally) pushes it to the stack
    struct cmark_node* NewNode(int nNodeType, bool bPushStack);

protected:
    //! Stores the current session of the exporter
    std::unique_ptr<CCommonMarkExporterSession> m_pSession;

    //! Gets a pointer to the root document node of the current session
    struct cmark_node* GetRootNode();

    //! Gets a pointer to the node on the top of the current session's stack
    struct cmark_node* TopNode();

    //! Pushes a node to the top of the stack
    void PushNode(struct cmark_node* pNode);

    //! Gets a reference to the output stream associated to the current session 
    std::ostream& GetStream() const;

public:
    CCommonMarkExporter();
    ~CCommonMarkExporter();

    /* Heading */

    //! @copydoc IDocExporter::GetHeading
    int GetHeading() const override;

    //! @copydoc IDocExporter::IncrementHeading
    void IncrementHeading() override;

    //! @copydoc IDocExporter::DecrementHeading
    void DecrementHeading() override;

    //! @copydoc IDocExporter::Heading
    bool Heading() override;

    //! @copydoc IDocExporter::PopStack
    bool PopStack() override;

    /* Style */

    //! @copydoc IDocExporter::Strong
    bool Strong() override;

    //! @copydoc IDocExporter::Emphasis
    bool Emphasis() override;

    /* Content */

    //! @copydoc IDocExporter::Paragraph
    bool Paragraph() override;

    //! @copydoc IDocExporter::Text
    bool Text(const std::string&) override;

    //! @copydoc IDocExporter::FormField
    bool FormField(const std::string& strLabel, const std::string& strValue, bool bCode) override;

    //! @copydoc IDocExporter::List
    bool List() override;

    //! @copydoc IDocExporter::Item
    bool Item() override;

    //! @copydoc IDocExporter::Code
    bool Code(const std::string& strText) override;

    //! @copydoc IDocExporter::Html
    bool Html(const std::string& strHtml) override;

    //! @copydoc IDocExporter::Collapsible
    bool Collapsible() override;

    //! @copydoc IDocExporter::Summary
    bool Summary() override;

    /* Control */

    //! @copydoc IDocExporter::Start
    bool Start(std::ostream& rOutput) override;
};

#endif