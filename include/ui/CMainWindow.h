#ifndef _CMAINWINDOW_H_
#define _CMAINWINDOW_H_

#include "wxExport/IMainWindow.h"

class CMainWindow : public IMainWindow
{
public:
    CMainWindow();
    ~CMainWindow();

protected:
	void ReloadProject();

	void onBtnNewProject(wxCommandEvent& event) override;
	void onBtnOpenProject(wxCommandEvent& event) override;
	void onBtnSaveProject(wxCommandEvent& event) override;
	void onBtnSaveAsProject(wxCommandEvent& event) override;
	void onBtnRunProject(wxCommandEvent& event) override;
	void onBtnStopProject(wxCommandEvent& event) override;
	void onBtnNewItem(wxCommandEvent& event) override;
	void onBtnDeleteItem(wxCommandEvent& event) override;
	void onBtnItemUp(wxCommandEvent& event) override;
	void onBtnItemDown(wxCommandEvent& event) override;
};

#endif