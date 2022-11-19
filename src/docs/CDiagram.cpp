#include "docs/CDiagram.h"
#include "util/Log.h"
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/gvplugin_render.h>

//! @brief Importing of GV plugins
#define GV_PLUGIN_API extern "C" __declspec(dllimport)

GV_PLUGIN_API gvplugin_library_t gvplugin_core_LTX_library;
GV_PLUGIN_API gvplugin_library_t gvplugin_gdiplus_LTX_library;
GV_PLUGIN_API gvplugin_library_t gvplugin_dot_layout_LTX_library;

//! @brief List of plug-ins to load
static lt_symlist_t lt_preloaded_symbols[] =
{
    { "gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library},
    { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library},
    { "gvplugin_gdiplus_LTX_library", &gvplugin_gdiplus_LTX_library},
    { 0, 0}
};

//! @brief RAII for the GraphViz context
class CGVContext
{
private:
    //! Stores the pointer to the context
    std::unique_ptr<GVC_t, int(*)(GVC_t*)> m_pGVC;

public:
    //! @brief Creates a GV contexts from the preloaded plug-ins
    CGVContext()
        : m_pGVC{ gvContextPlugins(lt_preloaded_symbols, false), gvFreeContext }
    {
        // Modify SVG engine to not output certain elements
        gvplugin_api_t* pApi = nullptr;
        for (unsigned int i = 0; (pApi = &gvplugin_core_LTX_library.apis[i])->types != nullptr; i++)
        {
            if (pApi->api != API_render)
                continue;

            if (0 == strcmp(pApi->types->type, "svg"))
            {
                gvrender_engine_t* pEngine = static_cast<gvrender_engine_t*>(pApi->types->engine);

                pEngine->begin_job = nullptr;
                pEngine->comment = nullptr;
            }
        }
    }

    //! @brief Check if the pointer is non-null
    operator bool() const
    {
        return m_pGVC.operator bool();
    }

    //! @brief Gets a non-owning pointer to the context
    operator GVC_t* ()
    {
        return m_pGVC.get();
    }

    //! @brief Gets a non-owning pointer to the context
    operator const GVC_t* () const
    {
        return m_pGVC.get();
    }
};

//! @brief RAII for graph layout
class CGvLayout
{
protected:
    //! Pointer to the context used for the layout
    GVC_t* const m_pGVC;

    //! Pointer to the graph being layed-out
    graph_t* const m_pGraph;

public:
    //! @brief Starts the layout of the graph using the requested engine
    CGvLayout(GVC_t* pGVC, graph_t* pGraph, const char* szEngine)
        : m_pGVC(pGVC)
        , m_pGraph(pGraph)
    {
        const int nStatus = gvLayout(m_pGVC, m_pGraph, szEngine);

        if (0 != nStatus)
            CWARNING("Failed to gvLayout graph: 0x%X", nStatus);
    }

    ~CGvLayout()
    {
        gvFreeLayout(m_pGVC, m_pGraph);
    }

    //! @brief Renders the layout to a file pointer using the provided format
    //! @return True if success
    bool Render(const char* szEngine, FILE* fp)
    {
        const int nStatus = gvRender(m_pGVC, m_pGraph, szEngine, fp);
        const bool bStatus = (0 == nStatus);

        if (!bStatus)
            CERROR("Failed to gvRender: 0x%X", nStatus);

        return bStatus;
    }

    //! @brief Renders the layout to a data buffer using the provided format
    //! @return True if success
    bool RenderData(const char* szFormat, std::string& strData)
    {
        char* pResult = nullptr;
        unsigned int uLength = 0;

        const int nStatus = gvRenderData(m_pGVC, m_pGraph, szFormat, &pResult, &uLength);
        const bool bStatus = (0 == nStatus);

        if (!bStatus)
            CERROR("Failed to gvRenderData: 0x%X", nStatus);
        else
        {
            strData.assign(pResult, uLength);
            gvFreeRenderData(pResult);
        }

        return bStatus;
    }
};

/* CDiagram */

CDiagram::CDiagram(const std::string& strName)
    : m_pGraph(agopen((char*)strName.c_str(), Agdirected, nullptr), agclose)
{
    // Set default attributes
    Agraph_t* pGraph = m_pGraph.get();
    agattr(pGraph, AGRAPH, "bgcolor", "transparent");
    agattr(pGraph, AGRAPH, "center", "true");

    agattr(pGraph, AGNODE, "shape", "rectangle");
    agattr(pGraph, AGNODE, "fillcolor", "white");
    agattr(pGraph, AGNODE, "style", "rounded,filled");
}

CDiagram::~CDiagram()
{

}

Agnode_s* CDiagram::Node(const std::string& strName)
{
    return agnode(m_pGraph.get(), (char*)strName.c_str(), true);
}

Agedge_s* CDiagram::Edge(const std::string& strName, Agnode_s* pNode1, Agnode_s* pNode2)
{
    return agedge(m_pGraph.get(), pNode1, pNode2, (char*)strName.c_str(), true);
}

bool CDiagram::ExportSVG(std::string& strData)
{
    bool bStatus = false;

    CGVContext gvc;

    if (!gvc)
        CERROR("Failed to create Graphviz context");
    else
    {
        const char* szEngine = "dot";

        // The first call to gvLayout lays out the graph
        // The first call to gvRender attaches the computed position information to the nodes and edges
        {
            CGvLayout cLayout(gvc, m_pGraph.get(), szEngine);
            bStatus = cLayout.Render(szEngine, nullptr);
        }

        // The second call to gvLayout adds straight-line edges to the already positioned nodes
        // the second call to gvRender outputs the graph.
        if (bStatus)
        {
            CGvLayout cLayout(gvc, m_pGraph.get(), szEngine);
            bStatus = cLayout.RenderData("svg", strData);
        }
    }

    return bStatus;
}