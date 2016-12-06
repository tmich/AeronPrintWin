#include "stdafx.h"
#include "AeronPrintApp.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	AeronPrintApp MyApp;

	// Run the application
	return MyApp.Run();
}