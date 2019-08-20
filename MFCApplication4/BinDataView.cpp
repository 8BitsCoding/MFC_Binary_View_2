// BinDataView.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication4.h"
#include "BinDataView.h"


// BinDataView

IMPLEMENT_DYNAMIC(BinDataView, CWnd)

BinDataView::BinDataView()
{

}

BinDataView::~BinDataView()
{
}


BEGIN_MESSAGE_MAP(BinDataView, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// BinDataView 메시지 처리기




int BinDataView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)	return -1;

	GetClientRect(&m_view_rect);
	m_view_cx = m_view_rect.Width();
	m_view_cy = m_view_rect.Height();

	m_item_view_count = m_view_cy / ITEM_HEIGHT;
	if (m_view_cy % ITEM_HEIGHT)	m_item_view_count++;

	m_draw_image.Create(m_view_rect.Width(), m_view_rect.Height(), 32);
	m_draw_dc.Attach(m_draw_image.GetDC());

	m_font.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"굴림");

	m_bk_brush.CreateSolidBrush(RGB(61, 77, 103));
	m_bk_pen.CreatePen(PS_SOLID, 1, RGB(61, 77, 103));
	m_grid_pen.CreatePen(PS_SOLID, 1, RGB(71, 97, 123));
	
	DrawData();

	return 0;
}

void BinDataView::DrawData()
{
	m_draw_dc.SelectObject(&m_bk_brush);
	m_draw_dc.SelectObject(&m_bk_pen);
	m_draw_dc.Rectangle(m_view_rect);

	m_draw_dc.SetBkColor(RGB(61, 77, 103));
	m_draw_dc.SetTextColor(RGB(0, 200, 255));

	if (!m_line_count) return;

	int scroll_pos = GetScrollPos(SB_VERT), count, last_flag = 0, i;

	if (m_item_view_count < m_line_count) {
		count = m_item_view_count;
		if (count + scroll_pos >= m_line_count) count--;
	}
	else count = m_line_count;

	// 마지막 줄을 출력해야 하는 경우
	// 마지막 줄인 경우 데이터가 m_data_count_per_line을 채우지 못하는지 체크
	if ((scroll_pos + count) == m_line_count && m_last_count) {
		last_flag = 1;
		count--;
	}

	m_draw_dc.SelectObject(&m_grid_pen);
	m_draw_dc.SelectObject(&m_font);

	for (i = 0; i < count; i++) {
		DrawOneLine(i, scroll_pos, m_data_count_per_line);
	}

	if (last_flag) DrawOneLine(i, scroll_pos, m_last_count);
}


void BinDataView::DrawOneLine(int a_line_index, int a_scroll_pos, int a_show_count)
{
	int len = swprintf_s(m_temp_string, 128, L"%06d :", a_line_index + a_scroll_pos + 1);
	m_draw_dc.SetTextColor(RGB(168, 168, 168));
	m_draw_dc.TextOut(5, a_line_index * ITEM_HEIGHT + 5, m_temp_string, len);

	unsigned char * p = mp_bin_data + (a_line_index + a_scroll_pos)*m_data_count_per_line;
	m_draw_dc.SetTextColor(RGB(255, 255, 255));
	for (int i = 0; i < a_show_count; i++) {
		swprintf_s(m_temp_string, 128, L"%02X", *p);
		m_draw_dc.TextOutW(60 + i * 24, a_line_index * ITEM_HEIGHT + 5, m_temp_string, 2);
		p++;
	}

	m_draw_dc.MoveTo(3, (a_line_index + 1)*ITEM_HEIGHT - 2);
	m_draw_dc.LineTo(m_view_cx - 3, (a_line_index + 1)*ITEM_HEIGHT - 2);
}


void BinDataView::OnDestroy()
{
	CWnd::OnDestroy();

	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
	m_draw_image.Destroy();

	m_bk_brush.DeleteObject();
	m_bk_pen.DeleteObject();
	m_grid_pen.DeleteObject();

	if (mp_bin_data != NULL) ::HeapFree(GetProcessHeap(), 0, mp_bin_data);

}


void BinDataView::OnPaint()
{
	CPaintDC dc(this); 

	m_draw_image.Draw(dc, 0, 0);
}


void BinDataView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == NULL) {
		ProcessScroll(nSBCode, nPos, SB_VERT);

		DrawData();
		Invalidate(FALSE);
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void BinDataView::Redraw()
{
	DrawData();

	CClientDC dc(this);
	m_draw_image.Draw(dc, 0, 0);
}


void BinDataView::ProcessScroll(UINT nSBCode, UINT nPos, int a_scroll_type)
{
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK) {
		//SetScrollPos(a_scroll_type, nPos);
		// 데이터의 수가 많아(3만개 이상)지만 nPos값이 제대로 들어오지 않음
		SCROLLINFO si = { sizeof(SCROLLINFO), SIF_TRACKPOS, };
		GetScrollInfo(SB_VERT, &si);
		SetScrollPos(a_scroll_type, si.nTrackPos);
		// 직접 스크롤 포지션을 얻는다.
	}
	else
	{
		int scroll_pos = GetScrollPos(a_scroll_type), min = 0, max = 0;
		GetScrollRange(a_scroll_type, &min, &max);
		switch (nSBCode) {
		case SB_LINEDOWN:
			if (scroll_pos < max) SetScrollPos(a_scroll_type, scroll_pos + 1);
			break;
		case SB_LINEUP:
			if (scroll_pos > min) SetScrollPos(a_scroll_type, scroll_pos - 1);
			break;
		case SB_PAGEDOWN:
			if (scroll_pos + 10 < max) SetScrollPos(a_scroll_type, scroll_pos + 10);
			break;
		case SB_PAGEUP:
			if (scroll_pos - 10 < min) SetScrollPos(a_scroll_type, scroll_pos + 10);
			break;
		}
	}
}

void BinDataView::LoadFile(const wchar_t * ap_path)
{
	if (mp_bin_data != NULL) ::HeapFree(::GetProcessHeap(), 0, mp_bin_data);

	FILE * p_file = NULL;
	if (0 == _wfopen_s(&p_file, ap_path, L"rb")) {
		fseek(p_file, 0, SEEK_END);
		// 파일의 끝으로 이동
		m_bin_data_size = ftell(p_file);
		// 끝으로 이동하면 해당 파일의 크기가 나온다.
		mp_bin_data = (unsigned char *)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, m_bin_data_size);
		// mp_bin_data = new unsinged char[m_bin_data_size];
		// 할당해야할 메모리가 클 경우 OS에서 직접관리하는 HeapAlloc이 더 안정적이다.
		fseek(p_file, 0, SEEK_SET);
		// 파일의 처음으로 이동
		fread(mp_bin_data, m_bin_data_size, 1, p_file);
		// 파일을 읽는다(복사한다)
		fclose(p_file);

		m_line_count = m_bin_data_size / m_data_count_per_line;
		m_last_count = m_bin_data_size % m_data_count_per_line;
		if (m_last_count) m_line_count++;
	}
	else {
		mp_bin_data = NULL;
		m_bin_data_size = 0;
		m_last_count = 0;
	}

	if (m_line_count > m_item_view_count) {
		ModifyStyle(0, WS_VSCROLL, SWP_DRAWFRAME);
		// 지울건 없고(0), 추가할것은 (WS_VSCROl)
		SetScrollRange(SB_VERT, 0, m_line_count - m_item_view_count + 1, 0);
	}
	else {
		ModifyStyle(WS_VSCROLL, 0, SWP_DRAWFRAME);
		// 지울건(WS_VSCROLL) 추가할건 없음(0)

		SetScrollPos(SB_VERT, 0, 1);
		Redraw();
	}
}