#include "ui/CAcceleratorEntry.h"

CAcceleratorEntry::CAcceleratorEntry(int nFlags, int nKeyCode, wxToolBarToolBase* pTool)
    : wxAcceleratorEntry(nFlags, nKeyCode, (pTool ? pTool->GetId() : 0))
{
    if (pTool)
    {
        const wxString strLabel = pTool->GetLabel();
        const wxString strHotkey = ToString();
        const wxString strHelp = strLabel + " (" + strHotkey + ")";

        pTool->SetShortHelp(strHelp);
    }
}