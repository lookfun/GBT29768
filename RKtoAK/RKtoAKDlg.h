
// RKtoAKDlg.h : 头文件
//

#pragma once


// CRKtoAKDlg 对话框
class CRKtoAKDlg : public CDialogEx
{
// 构造
public:
	CRKtoAKDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RKTOAK_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CString csRK1;
	CString csRK2;
	CString csRK3;
	CString csRK4;
	CString csTID1;
	CString csTID2;
	CString csAK1;
	CString csAK2;
	CString csAK3;
	CString csAK4;
	afx_msg void OnBnClickedButton2();
};
