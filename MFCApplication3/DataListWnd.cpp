// DataListWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "DataListWnd.h"


// DataListWnd

IMPLEMENT_DYNAMIC(DataListWnd, CWnd)

DataListWnd::DataListWnd()
{

}

DataListWnd::~DataListWnd()
{
}


BEGIN_MESSAGE_MAP(DataListWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// DataListWnd 메시지 처리기




int DataListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	ZeroMemory(m_data_list, sizeof(m_data_list));
	GetClientRect(&m_view_rect);

	m_view_cx = m_view_rect.Width();
	m_view_cy = m_view_rect.Height();
	m_item_view_count = m_view_cy / ITEM_HEIGHT;
	if (m_view_cy % ITEM_HEIGHT) {
		m_item_view_count++;
	}

	SetScrollRange(SB_VERT, 0, MAX_GROUP_COUNT - m_item_view_count + 1, 0);
	
	m_draw_image.Create(m_view_rect.Width(), m_view_rect.Height(), 32);
	m_draw_dc.Attach(m_draw_image.GetDC());

	m_bk_brush.CreateSolidBrush(RGB(61, 77, 103));
	m_bk_pen.CreatePen(PS_SOLID, 1, RGB(61, 77, 103));
	m_grid_pen.CreatePen(PS_SOLID, 1, RGB(71, 97, 123));

	DrawData();

	return 0;
}

void DataListWnd::DrawData()
{
	m_draw_dc.SelectObject(&m_bk_brush);
	m_draw_dc.SelectObject(&m_bk_pen);

	m_draw_dc.Rectangle(m_view_rect);

	m_draw_dc.SetBkColor(RGB(61, 77, 103));
	m_draw_dc.SetTextColor(RGB(0, 200, 255));

	int scroll_pos = GetScrollPos(SB_VERT), count = m_item_view_count;
	if (count + scroll_pos >= MAX_GROUP_COUNT) count--;

	m_draw_dc.SelectObject(&m_grid_pen);

	int * p, len;

	if (m_state_check) {
		for (int i = 0; i < count; i++)
		{
			p = m_data_list[i];
			m_draw_dc.SetTextColor(RGB(190, 190, 190));
			len = swprintf_s(m_temp_string, 128, L"Group %03d :", i + scroll_pos + 1);
			m_draw_dc.TextOutW(5, i * ITEM_HEIGHT + 2, m_temp_string, len);

			for (int sub_i = 0; sub_i < MAX_VALUE_COUNT; sub_i++)
			{
				len = swprintf_s(m_temp_string, 128, L"%03d, ", *p);
				if(*p < 100) m_draw_dc.SetTextColor(RGB(0, 255, 100));
				else if (*p < 900) m_draw_dc.SetTextColor(RGB(255, 0, 100));
				else m_draw_dc.SetTextColor(RGB(0, 255, 100));
				m_draw_dc.TextOutW(84 + sub_i*32, i * ITEM_HEIGHT + 2, m_temp_string, len);
				p++;
			}

			m_draw_dc.MoveTo(3, (i + 1)*ITEM_HEIGHT - 2);
			m_draw_dc.LineTo(m_view_cx - 3, (i + 1)*ITEM_HEIGHT - 2);
		}
	}
	else {
		for (int i = 0; i < count; i++)
		{
			p = m_data_list[i];
			len = swprintf_s(m_temp_string, 128, L"Group %03d : %03d, %03d, %03d, %03d, %03d",
				i + scroll_pos + 1, *p, *(p + 1), *(p + 2), *(p + 3), *(p + 4));

			m_draw_dc.TextOutW(5, i * ITEM_HEIGHT + 2, m_temp_string, len);
			m_draw_dc.MoveTo(3, (i + 1)*ITEM_HEIGHT - 2);
			m_draw_dc.LineTo(m_view_cx - 3, (i + 1)*ITEM_HEIGHT - 2);
		}
	}

}

void DataListWnd::OnDestroy()
{
	CWnd::OnDestroy();

	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
	m_draw_image.Destroy();

	m_bk_brush.DeleteObject();
	m_bk_pen.DeleteObject();
	m_grid_pen.DeleteObject();

}


void DataListWnd::OnPaint()
{
	CPaintDC dc(this); 

	m_draw_image.Draw(dc, 0, 0);
}


void DataListWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == NULL) {
		ProcessScroll(nSBCode, nPos, SB_VERT);
		DrawData();
		Invalidate(FALSE);
	}
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void DataListWnd::UpdateData(int a_index, int * ap_data)
{
	if (a_index < MAX_GROUP_COUNT) {
		memcpy(m_data_list + a_index, ap_data, sizeof(int)*MAX_VALUE_COUNT);
	}
}

void DataListWnd::Redraw()
{
	DrawData();

	CClientDC dc(this);
	m_draw_image.Draw(dc, 0, 0);
}

void DataListWnd::ProcessScroll(UINT nSBCode, UINT nPos, int a_scroll_type)
{
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK) {
		SetScrollPos(a_scroll_type, nPos);
	}
	else {
		int scroll_pos = GetScrollPos(a_scroll_type), min = 0, max = 0;
		GetScrollRange(a_scroll_type, &min, &max);
		switch (nSBCode) {
		case SB_LINEDOWN:
			if (scroll_pos < max) SetScrollPos(a_scroll_type, scroll_pos+1);
			break;
		case SB_LINEUP:
			if (scroll_pos > min) SetScrollPos(a_scroll_type, scroll_pos - 1);
			break;
		case SB_PAGEDOWN:
			if (scroll_pos + 10 < max) SetScrollPos(a_scroll_type, scroll_pos + 10);
			break;
		case SB_PAGEUP:
			if (scroll_pos - 10 > min) SetScrollPos(a_scroll_type, scroll_pos - 10);
			break;
		}
	}
}

void DataListWnd::SetStateCheck(int a_state)
{
	m_state_check = a_state;
	Redraw();
}