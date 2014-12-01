
// GBT29768Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CGBT29768Dlg 对话框
class CGBT29768Dlg : public CDialogEx
{
// 构造
public:
	CGBT29768Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GBT29768_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
	afx_msg LRESULT OnListShow(WPARAM wParam,LPARAM lParam);
public:
	CListBox m_List;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedAuthstart();
	u32 m_AKX;
	CString csRK1;
	CString csRK2;
	CString csRK3;
	CString csRK4;
	int RKorAK;
};
