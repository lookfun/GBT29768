
// TagDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CTagDlg 对话框
class CTagDlg : public CDialogEx
{
// 构造
public:
	CTagDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TAG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	u32 m_TID1;
	u32 m_TID2;
	afx_msg void OnBnClickedConnect();
	afx_msg LRESULT OnListShow(WPARAM wParam,LPARAM lParam);
	u32 m_AKX;
	int m_AuthMode;
	CListBox m_List;
	CString csAKS1;
	CString csAKS2;
	CString csAKS3;
	CString csAKS4;
	afx_msg void OnBnClickedSet();
};
