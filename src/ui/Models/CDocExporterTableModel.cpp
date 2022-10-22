#include "ui/Models/CDocExporterTableModel.h"
#include "core/CProject.h"
#include "ui/SDocExporterTypeInfo.h"

/* static */ IDocExporter* CDocExporterTableModel::GetPointer(const wxDataViewItem& item)
{
    return static_cast<IDocExporter*>(item.GetID());
}

/* static */ wxDataViewItem CDocExporterTableModel::GetViewItem(const IDocExporter* pArg)
{
    return wxDataViewItem((void*)pArg);
}

class CExporterTypeModelColumnHandler : public IModelColumnHandler
{
public:
    CExporterTypeModelColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const IDocExporter* const pItem = CDocExporterTableModel::GetPointer(rItem);
        assert(pItem);

        const wxString strDesc = SDocExporterTypeInfo::GetName(pItem->GetType());
        value = strDesc;
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        return false;
    }
};

class CExporterDestinationModelColumnHandler : public IModelColumnHandler
{
public:
    CExporterDestinationModelColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const IDocExporter* const pItem = CDocExporterTableModel::GetPointer(rItem);
        assert(pItem);

        const wxString strDesc = pItem->GetOutputFileName();
        value = strDesc;
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        IDocExporter* const pItem = CDocExporterTableModel::GetPointer(rItem);
        assert(pItem);

        wxString strPath = value.GetString();
        pItem->SetOutputFileName(strPath.ToStdString());

        return true;
    }
};

/* CDocExporterTableModel */
CDocExporterTableModel::CDocExporterTableModel(CProject& rEdit)
    : m_rEdit(rEdit)
{
    // Create columns
    m_mapColumns[EColumn::Type]        = std::make_unique<CExporterTypeModelColumnHandler>();
    m_mapColumns[EColumn::Destination] = std::make_unique<CExporterDestinationModelColumnHandler>();
}

CDocExporterTableModel::~CDocExporterTableModel()
{

}

const wxDataViewItem CDocExporterTableModel::NewItem(EDocExporter eType)
{
    CProject::vec_exporters_t& rvExporters = m_rEdit.GetDocumentationExporters();
    rvExporters.emplace_back( SDocExporterTypeInfo::New(eType) );

    const IDocExporter* const pNewItem = rvExporters.back().get();
    const wxDataViewItem cItem = GetViewItem(pNewItem);

    ItemAdded(GetParent(cItem), cItem);

    return cItem;
}

bool CDocExporterTableModel::DeleteItem(const wxDataViewItem& cDeleteItem)
{
    bool bSuccess = false;
    const IDocExporter* const pDeleteItem = GetPointer(cDeleteItem);

    if (nullptr != pDeleteItem)
    {

        CProject::vec_exporters_t& rvExporters = m_rEdit.GetDocumentationExporters();
        CProject::vec_exporters_t::const_iterator itFound = std::find_if(rvExporters.cbegin(), rvExporters.cend(),
            [pDeleteItem](const CProject::ptr_exporter_t& pSearch)->bool {
                return (pDeleteItem == pSearch.get());
            }
        );

        if (rvExporters.end() != itFound)
        {
            rvExporters.erase(itFound);
            ItemDeleted(GetParent(cDeleteItem), cDeleteItem);
            bSuccess = true;
        }
    }

    return bSuccess;
}

IModelColumnHandler* CDocExporterTableModel::GetColumnInfo(unsigned int nModelColumn)
{
    IModelColumnHandler* pInfo = nullptr;

    const auto itFound = m_mapColumns.find((EColumn)nModelColumn);
    if (m_mapColumns.end() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

const IModelColumnHandler* CDocExporterTableModel::GetColumnInfo(unsigned int nModelColumn) const
{
    const IModelColumnHandler* pInfo = nullptr;

    const auto itFound = m_mapColumns.find((EColumn)nModelColumn);
    if (m_mapColumns.cend() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

unsigned int CDocExporterTableModel::GetColumnCount() const
{
    return (unsigned int)EColumn::Count;
}

wxDataViewItem CDocExporterTableModel::GetParent(const wxDataViewItem& item) const
{
    return GetViewItem(nullptr);
}

bool CDocExporterTableModel::IsContainer(const wxDataViewItem& item) const
{
    return false;
}

unsigned int CDocExporterTableModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uCount = 0;

    if (nullptr == parent.GetID())
    {
        for (const CProject::ptr_exporter_t& pExporter : m_rEdit.GetDocumentationExporters())
            array.push_back(GetViewItem(pExporter.get()));
    }

    return uCount;
}