#include "ui/SDocExporterTypeInfo.h"
#include "docs/CMarkdownExporter.h"
#include "docs/CHtmlExporter.h"

/* static */ const SDocExporterTypeInfo::map_type& SDocExporterTypeInfo::GetRegistry()
{
    static const map_type m_mapExporters
    {
        { EDocExporter::Markdown, 
            SDocExporterTypeInfo {
                "Markdown (.md)",
                &CMarkdownExporter::Create
            }
        },

        { EDocExporter::HTML,
            SDocExporterTypeInfo {
                "Web page (.html)",
                &CHtmlExporter::Create
            }
        },

        // { EDocExporter::LaTeX, 
        //     SDocExporterTypeInfo {
        //         "LaTeX (.tex)",
        //         // TODO: Implemente LaTeX exporter
        //     }
        // },
    };

    return m_mapExporters;
}

//! @brief Gets the type information of the requested exporter type
/* static */ const SDocExporterTypeInfo& SDocExporterTypeInfo::GetInfo(EDocExporter eType)
{
    return GetRegistry().at(eType);
}

//! @brief Gets the display name of the requested exporter type
/* static */ const wxString& SDocExporterTypeInfo::GetName(EDocExporter eType)
{
    return GetInfo(eType).strTypeName;
}

//! @brief Instantiates a new exporter of the requested type
/* static */ IDocExporter* SDocExporterTypeInfo::New(EDocExporter eType)
{
    const fnExporterFactory_t& fnFactory = GetInfo(eType).m_fnCreate;
    return fnFactory();
}
