#ifndef _CDIAGRAM_H_
#define _CDIAGRAM_H_

#include <memory>
#include <string>

struct Agraph_s;
struct Agnode_s;
struct Agedge_s;

//! @brief Represents a diagram of a graph
class CDiagram
{
protected:
    //! Holds the graph description
    std::unique_ptr<Agraph_s, int(*)(Agraph_s*)> m_pGraph;

    int Attribute(void* pItem, char* szName, const char* szValue, const char* szDefault);

public:
    //! @brief Creates a diagram with the provided name
    CDiagram(const std::string& strName);

    ~CDiagram();

    //! @brief Creates (or gets a pointer to) a node of with the provided name
    Agnode_s* Node(const std::string& strName);

    //! @brief Creates (or gets a pointer to) an edge between the given nodes
    Agedge_s* Edge(const std::string& strName, Agnode_s*, Agnode_s*);

    //! @brief Assigns a URL to the node
    static bool URL(Agnode_s*, const char* szURL);

    //! @brief Exports the graph in SVG format to the provided buffer
    //! @return True if success
    bool ExportSVG(std::string& strData);
};

#endif