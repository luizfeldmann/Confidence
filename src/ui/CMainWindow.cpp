#include "ui/CMainWindow.h"

CMainWindow::CMainWindow() :
    IMainWindow(nullptr)
{
    SetIcon(wxIcon("RES_ID_ICON_APPLICATION"));
}

CMainWindow::~CMainWindow()
{

}