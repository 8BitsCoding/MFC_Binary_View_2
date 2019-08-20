
// MFCApplication4.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "MFCApplication4.h"
#include "MFCApplication4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication4App

BEGIN_MESSAGE_MAP(CMFCApplication4App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCApplication4App 생성

CMFCApplication4App::CMFCApplication4App()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CMFCApplication4App 개체입니다.

CMFCApplication4App theApp;


// CMFCApplication4App 초기화

BOOL CMFCApplication4App::InitInstance()
{
	
	CMFCApplication4Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	return FALSE;
}

