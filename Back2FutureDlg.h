// Back2FutureDlg.h : header file
//

#if !defined(AFX_BACK2FUTUREDLG_H__3EDBD407_7371_4CBF_A1B5_164C6D74A0B0__INCLUDED_)
#define AFX_BACK2FUTUREDLG_H__3EDBD407_7371_4CBF_A1B5_164C6D74A0B0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define theDlg (*theApp.pdlg)

#define _LogCR(X) theApp.pdlg->LogCR(X)
#define _LOGCR(X) theApp.pdlg->LogCR(_T(X))

/////////////////////////////////////////////////////////////////////////////
// CBack2FutureDlg dialog

class CBack2FutureDlg : public CDialog
{
// Construction
public:
	CBack2FutureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBack2FutureDlg)
	enum { IDD = IDD_BACK2FUTURE_DIALOG };
	CStatic	m_StaticStatus;
	CEdit	m_EditStatus;
	CButton	m_ButtonFuture;
	CButton	m_ButtonLaunch;
	CButton	m_ButtonPast;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBack2FutureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBack2FutureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLaunch();
	afx_msg void OnButtonFuture();
	afx_msg void OnButtonPast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Alex
private:
	CString cmdLine;
	CString infoAppName;
	SYSTEMTIME timeNow;
	SYSTEMTIME timePast;
	void PrintLastError(void);

public:
	BOOL debug;

	void CmdLine(LPCTSTR);

	void Log(LPCTSTR);
	void LogCR(LPCTSTR);
	void ThreadEntryPoint(LPVOID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACK2FUTUREDLG_H__3EDBD407_7371_4CBF_A1B5_164C6D74A0B0__INCLUDED_)
