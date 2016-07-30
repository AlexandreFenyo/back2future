// Back2Future.h : main header file for the BACK2FUTURE application
//

#if !defined(AFX_BACK2FUTURE_H__BE0247C1_6B87_4A53_99EB_8ABD957F1CC7__INCLUDED_)
#define AFX_BACK2FUTURE_H__BE0247C1_6B87_4A53_99EB_8ABD957F1CC7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBack2FutureApp:
// See Back2Future.cpp for the implementation of this class
//

class CBack2FutureApp : public CWinApp
{
public:
	CBack2FutureApp();

	class CBack2FutureDlg *pdlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBack2FutureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBack2FutureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CBack2FutureApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACK2FUTURE_H__BE0247C1_6B87_4A53_99EB_8ABD957F1CC7__INCLUDED_)
