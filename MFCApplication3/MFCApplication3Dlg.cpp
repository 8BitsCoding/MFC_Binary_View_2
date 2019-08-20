
// MFCApplication3Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication3Dlg 대화 상자



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATE_CHECK, m_state_check);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START_BTN, &CMFCApplication3Dlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_STOP_BTN, &CMFCApplication3Dlg::OnBnClickedStopBtn)
	ON_BN_CLICKED(IDC_STATE_CHECK, &CMFCApplication3Dlg::OnBnClickedStateCheck)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 메시지 처리기

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CRect r;
	GetDlgItem(IDC_LIST_RECT)->GetWindowRect(r);
	ScreenToClient(r);
	m_data_list.CreateEx(WS_EX_STATICEDGE, NULL, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL, r, this, 2005);

	SetDlgItemInt(IDC_UPDATE_TIME_EDIT, 100);

	srand((unsigned int)time(NULL));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication3Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	OnBnClickedStopBtn();
	m_data_list.DestroyWindow();
}


void CMFCApplication3Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent) {
		int data[MAX_VALUE_COUNT];
		for (int i = 0; i < MAX_GROUP_COUNT; i++) {
			for (int sub_i = 0; sub_i < MAX_VALUE_COUNT; sub_i++)	data[sub_i] = rand() % 1000;
			m_data_list.UpdateData(i, data);
		}
		m_data_list.Redraw();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMFCApplication3Dlg::OnBnClickedStartBtn()
{
	SetTimer(1, GetDlgItemInt(IDC_UPDATE_TIME_EDIT), NULL);
}


void CMFCApplication3Dlg::OnBnClickedStopBtn()
{
	KillTimer(1);
}


void CMFCApplication3Dlg::OnBnClickedStateCheck()
{
	m_data_list.SetStateCheck(m_state_check.GetCheck());
}
