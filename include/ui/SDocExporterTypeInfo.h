#ifndef _SDOCEXPORTERTYPEINFO_
#define _SDOCEXPORTERTYPEINFO_

#include "docs/IDocExporter.h"
#include <wx/string.h>
#include <functional>
#include <map>

//! @brief Presentation & creation information on document exporters
struct SDocExporterTypeInfo
{
    using fnExporterFactory_t = std::function<IDocExporter* ()>;

    wxString strTypeName;           //!< Display name of the document exporter
    fnExporterFactory_t m_fnCreate; //!< Factory function to create a new instance of that type

    using map_type = std::map<EDocExporter, SDocExporterTypeInfo>;

    //! @brief Gets the full registry of all available exporters and their associated type information
    static const map_type& GetRegistry();

    //! @brief Gets the type information of the requested exporter type
    static const SDocExporterTypeInfo& GetInfo(EDocExporter eType);

    //! @brief Gets the display name of the requested exporter type
    static const wxString& GetName(EDocExporter eType);

    //! @brief Instantiates a new exporter of the requested type
    static IDocExporter* New(EDocExporter eType);
};

#endif