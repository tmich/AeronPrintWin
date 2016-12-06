#pragma once
#include "MainFrame.h"

class AeronPrintApp :
	public CWinApp
{
public:
	AeronPrintApp();
	virtual ~AeronPrintApp();

protected:
	virtual BOOL InitInstance() override;

private:
	CMainFrame m_frame;
};

