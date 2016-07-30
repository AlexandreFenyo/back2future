
// Back2FutureDlg.cpp : implementation file

#include "stdafx.h"
#include "Back2Future.h"
#include "Back2FutureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOGCR(X) LogCR(_T(X))
#define LOG(X) Log(_T(X))

static CBack2FutureDlg *pb2fDlg;

/////////////////////////////////////////////////////////////////////////////
// CBack2FutureDlg functions

UINT ThreadFunc(LPVOID pParam)
{
	pb2fDlg->ThreadEntryPoint(pParam);
	return 0;
}

// Background job
void CBack2FutureDlg::ThreadEntryPoint(LPVOID pParam)
{
	DWORD exitCode;

    HANDLE hProc = OpenProcess(0, FALSE, (DWORD) pParam);
	if (hProc == NULL) {
	  PrintLastError();
	  return;
	}

	while (TRUE) {
	  Sleep(10000); // 10 sec polling interval
	  BOOL ret = GetExitCodeProcess(hProc, &exitCode);
	  if (!ret) {
		PrintLastError();
		CloseHandle(hProc);
		return;
	  }
	  if (exitCode != STILL_ACTIVE) {
	    OnButtonFuture();
		CloseHandle(hProc);
		return;
	  }
	}
}

void CBack2FutureDlg::LogCR(LPCTSTR s)
{
	CString str;
	
	m_EditStatus.GetWindowText(str);
	m_EditStatus.SetWindowText(str + s + _T("\r\n"));

	m_EditStatus.LineScroll(m_EditStatus.GetLineCount() - 6);
}

void CBack2FutureDlg::Log(LPCTSTR s)
{
	CString str;
	
	m_EditStatus.GetWindowText(str);
	m_EditStatus.SetWindowText(str + s);

	m_EditStatus.LineScroll(m_EditStatus.GetLineCount() - 6);
}

/////////////////////////////////////////////////////////////////////////////
// CBack2FutureDlg dialog

CBack2FutureDlg::CBack2FutureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBack2FutureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBack2FutureDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pb2fDlg = this;
}

void CBack2FutureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBack2FutureDlg)
	DDX_Control(pDX, IDC_STATIC1, m_StaticStatus);
	DDX_Control(pDX, IDC_EDIT1, m_EditStatus);
	DDX_Control(pDX, IDC_BUTTON3, m_ButtonFuture);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonLaunch);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonPast);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBack2FutureDlg, CDialog)
	//{{AFX_MSG_MAP(CBack2FutureDlg)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonLaunch)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonFuture)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonPast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBack2FutureDlg message handlers

BOOL CBack2FutureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	
	TRACE(_T("program started\n"));
	LOGCR("Application started\r\nTime reference : GMT");
	if (debug == TRUE) LOGCR("debug version");

	LOGCR("\r\nInfo file :");
	LogCR(cmdLine);

	m_ButtonPast.EnableWindow(TRUE);
	m_ButtonLaunch.EnableWindow(FALSE);
	m_ButtonFuture.EnableWindow(FALSE);

	FILE *f = _tfopen((cmdLine), _T("r"));
	if (f == NULL) {
		AfxMessageBox(_T("can't open file"));
		PostQuitMessage(1);
		return TRUE;
	}

	char buf[256];
	fgets(buf, sizeof(buf), f);
	if (ferror(f)) {
		AfxMessageBox(_T("can't read file"));
		PostQuitMessage(1);
		return TRUE;
	}
	if (buf[0]) buf[strlen(buf) - 1] = 0;
	infoAppName = buf;

	fgets(buf, sizeof(buf), f);
	if (ferror(f)) {
		AfxMessageBox(_T("can't read file"));
		PostQuitMessage(1);
		return TRUE;
	}
	if (buf[0]) buf[strlen(buf) - 1] = 0;
	CString infoDate(buf);

	fclose(f);

	timePast.wMilliseconds = 0;
	sscanf(buf, "%u/%u/%u %u:%u:%u",
		   &timePast.wYear, &timePast.wMonth, &timePast.wDay,
		   &timePast.wHour, &timePast.wMinute, &timePast.wSecond);

	LogCR(infoAppName);
	LogCR(infoDate);
	m_StaticStatus.SetWindowText(infoAppName);

	SYSTEMTIME systime;
	GetSystemTime(&systime);
	CString str;
	str.Format(_T("\r\nPresent: %u/%.2u/%.2u %.2u:%.2u:%.2u"),
		       systime.wYear, systime.wMonth, systime.wDay,
		       systime.wHour, systime.wMinute, systime.wSecond);
	LogCR(str);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBack2FutureDlg::CmdLine(LPCTSTR cmd)
{
    if ((_tcslen(cmd) >= 2) && (cmd[_tcslen(cmd) - 1] == '"') && (cmd[0] == '"')) {
	  cmdLine += &cmd[1];
	  cmdLine = cmdLine.Left(cmdLine.GetLength() - 1);
	} else cmdLine += cmd;
}

void CBack2FutureDlg::PrintLastError()
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
				  GetLastError(), 0, (LPTSTR) &lpMsgBuf, 0, NULL);
	::MessageBox(NULL, (LPCTSTR) lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
	LocalFree(lpMsgBuf);
}

void CBack2FutureDlg::OnButtonPast()
{
	m_ButtonPast.EnableWindow(FALSE);
	m_ButtonLaunch.EnableWindow(TRUE);
	m_ButtonFuture.EnableWindow(TRUE);

	GetSystemTime(&timeNow);
	CString str;
	str.Format(_T("Launch time: %u/%.2u/%.2u %.2u:%.2u:%.2u"),
		       timeNow.wYear, timeNow.wMonth, timeNow.wDay,
               timeNow.wHour, timeNow.wMinute, timeNow.wSecond);
	LogCR(str);

	str.Format(_T("Set time: %u/%.2u/%.2u %.2u:%.2u:%.2u"),
			    timePast.wYear, timePast.wMonth, timePast.wDay,
                timePast.wHour, timePast.wMinute, timePast.wSecond);
	LogCR(str);

	int ret = SetSystemTime(&timePast);
	if (!ret) PrintLastError();
}

void CBack2FutureDlg::OnButtonLaunch()
{
	m_ButtonPast.EnableWindow(FALSE);
	m_ButtonLaunch.EnableWindow(FALSE);
	m_ButtonFuture.EnableWindow(FALSE);

	wchar_t buf[256];
#ifndef  UNICODE
	mbstowcs(buf, infoAppName.GetBuffer(0), 256);
    infoAppName.ReleaseBuffer();
	buf[255] = 0;
#else
	wcscpy(buf, infoAppName.GetBuffer(0));
    infoAppName.ReleaseBuffer();
#endif

	PROCESS_INFORMATION procinfo;
	int ret = CreateProcess(buf, NULL, NULL, NULL, NULL, 0, NULL, NULL, NULL, &procinfo);
	if (!ret) PrintLastError();

	::AfxBeginThread((AFX_THREADPROC) ThreadFunc, (LPVOID) procinfo.dwProcessId);
}

void CBack2FutureDlg::OnButtonFuture()
{
	m_ButtonPast.EnableWindow(FALSE);
	m_ButtonLaunch.EnableWindow(FALSE);
	m_ButtonFuture.EnableWindow(FALSE);

	SYSTEMTIME now;
	GetSystemTime(&now);
	int delay = (now.wHour - timePast.wHour) * 3600 + (now.wMinute - timePast.wMinute) * 60 +
		        now.wSecond - timePast.wSecond;

	CString str;
	str.Format(_T("delay: %ds"), delay);
	LogCR(str);

	str.Format(_T("%u %u %u %u %u %u\r\n%u %u %u %u %u %u"),
		       now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
			   timePast.wYear, timePast.wMonth, timePast.wDay, timePast.wHour, timePast.wMinute, timePast.wSecond);

//	if (delay < 0) delay += 86400;
	if (delay < 0) {
	  LogCR(str);
	  MessageBox(_T("invalid delay"));
	  return;
	}
	if (delay > 1200) {
	  LogCR(str);
	  MessageBox(_T("invalid delay"));
	  return;
	}

	CTimeSpan span(0, 0, 0, delay);
	CTime newTime(timeNow);
	newTime += span;
	SYSTEMTIME t;
	int ret = newTime.GetAsSystemTime(t);
	if (!ret) {
	  PrintLastError();
	  return;
	}

	str.Format(_T("Set time: %u/%.2u/%.2u %.2u:%.2u:%.2u"),
		       t.wYear, t.wMonth, t.wDay,
               t.wHour, t.wMinute, t.wSecond);
	LogCR(str);
	SetSystemTime(&t);

	CTime fTime(now);
	CTimeSpan span2(0, 0, 0, 2);
	fTime += span2;
	fTime.GetAsSystemTime(t);
	str.Format(_T("%u/%.2u/%.2u %.2u:%.2u:%.2u"),
		       t.wYear, t.wMonth, t.wDay,
               t.wHour, t.wMinute, t.wSecond);

	FILE *f = _tfopen((cmdLine), _T("w+"));
	if (f == NULL) {
		AfxMessageBox(_T("can't open file"));
		PostQuitMessage(1);
		return;
	}
	_ftprintf(f, _T("%s\n%s\n"), infoAppName.GetBuffer(0), str.GetBuffer(0));
    infoAppName.ReleaseBuffer();
	fclose(f);
	LOGCR("Config file written.");
}

