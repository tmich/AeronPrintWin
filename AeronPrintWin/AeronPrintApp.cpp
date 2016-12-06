#include "stdafx.h"
#include "AeronPrintApp.h"



AeronPrintApp::AeronPrintApp()
{
}


AeronPrintApp::~AeronPrintApp()
{
}

BOOL AeronPrintApp::InitInstance()
{
	//Create the Frame Window
	if (m_frame.Create() == 0)
	{
		// We get here if the Frame creation fails

		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return false; // returning FALSE ends the application
	}

	return true;
}
