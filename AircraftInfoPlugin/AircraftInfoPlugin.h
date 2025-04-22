// AircraftInfoPlugin.h : main header file for the AircraftInfoPlugin DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAircraftInfoPluginApp
// See AircraftInfoPlugin.cpp for the implementation of this class
//

class CAircraftInfoPluginApp : public CWinApp
{
public:
	CAircraftInfoPluginApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
