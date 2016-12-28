#pragma once

class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

class CMainWindow : public CFrameWnd
{
public:
	CMainWindow();
protected:
	afx_msg void OnPaint();
	//afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg LRESULT OnPaint2(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};