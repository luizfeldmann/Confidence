#include "ui/CMainWindow.h"
#include "ui/resources.h"

CMainWindow::CMainWindow() :
    IMainWindow(nullptr)
{
    SetIcon(wxIcon(xstr(IDF_MAINICON)));
}

CMainWindow::~CMainWindow()
{

}