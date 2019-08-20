#pragma once

#define MAX_GROUP_COUNT 100
#define MAX_VALUE_COUNT 5
#define ITEM_HEIGHT		22

// DataListWnd

class DataListWnd : public CWnd
{
	DECLARE_DYNAMIC(DataListWnd)

private:
	CImage m_draw_image;
	CDC m_draw_dc;
	CRect m_view_rect;

	CBrush m_bk_brush;
	CPen m_bk_pen, m_grid_pen;

	int m_data_list[MAX_GROUP_COUNT][MAX_VALUE_COUNT];
	int m_view_cx, m_view_cy, m_item_view_count;

	wchar_t m_temp_string[128];

public:
	DataListWnd();
	virtual ~DataListWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void ProcessScroll(UINT nSBCode, UINT nPos, int a_scroll_type);
	void DrawData();
	void UpdateData(int a_index, int * ap_data);
	void Redraw();
	void SetStateCheck(int a_state);
	int m_state_check = 0;
};


