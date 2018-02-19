// easyViewer.h : main header file for the EASYVIEWER application
//

#if !defined(AFX_EASYVIEWER_H__F1A1C4B3_07A2_44B0_9F20_A1BCCE92D6C2__INCLUDED_)
#define AFX_EASYVIEWER_H__F1A1C4B3_07A2_44B0_9F20_A1BCCE92D6C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerApp:
// See easyViewer.cpp for the implementation of this class
//

class CEasyViewerApp : public CWinApp
{
public:
	CEasyViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEasyViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYVIEWER_H__F1A1C4B3_07A2_44B0_9F20_A1BCCE92D6C2__INCLUDED_)
