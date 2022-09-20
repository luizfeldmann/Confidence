#ifndef _CACCELERATORENTRY_H_
#define _CACCELERATORENTRY_H_

#include <wx/accel.h>
#include <wx/tbarbase.h>

class CAcceleratorEntry : public wxAcceleratorEntry
{
public:
    CAcceleratorEntry(int nFlags, int nKeyCode, wxToolBarToolBase* pTool);
};

#endif