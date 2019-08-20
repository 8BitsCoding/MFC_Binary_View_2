#pragma once

#define ITEM_HEIGHT 22
// BinDataView

class BinDataView : public CWnd
{
	DECLARE_DYNAMIC(BinDataView)

private:
	CImage m_draw_image;
	CDC m_draw_dc;
	CRect m_view_rect;

	CFont m_font;
	CBrush m_bk_brush;
	CPen m_bk_pen, m_grid_pen;

	int m_view_cx, m_view_cy, m_item_view_count, m_data_count_per_line = 24;
	wchar_t m_temp_string[1024];

	int m_bin_data_size = 0, m_line_count = 0, m_last_count = 0;
	unsigned char * mp_bin_data = NULL;

public:
	BinDataView();
	virtual ~BinDataView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void DrawData();
	void ProcessScroll(UINT nSBCode, UINT nPos, int a_scroll_type);
	void Redraw();
	void LoadFile(const wchar_t * ap_path);
	void DrawOneLine(int a_line_index, int a_scroll_pos, int a_show_count);
};


