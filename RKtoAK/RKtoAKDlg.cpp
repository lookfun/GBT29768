
// RKtoAKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RKtoAK.h"
#include "RKtoAKDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRKtoAKDlg 对话框



CRKtoAKDlg::CRKtoAKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRKtoAKDlg::IDD, pParent)
	, RK1(_T(""))
	, RK2(_T(""))
	, RK3(_T(""))
	, RK4(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRKtoAKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, csRK1);
	DDX_Text(pDX, IDC_EDIT2, csRK2);
	DDX_Text(pDX, IDC_EDIT3, csRK3);
	DDX_Text(pDX, IDC_EDIT4, csRK4);
}

BEGIN_MESSAGE_MAP(CRKtoAKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRKtoAKDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRKtoAKDlg 消息处理程序

BOOL CRKtoAKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRKtoAKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRKtoAKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRKtoAKDlg::OnBnClickedButton1()
{
	UpdateData(true);
	unsigned int RKS[4];

	char *RKS1=(LPSTR)(LPCTSTR)csRK1;
	char *RKS2=(LPSTR)(LPCTSTR)csRK2;
	char *RKS3=(LPSTR)(LPCTSTR)csRK3;
	char *RKS4=(LPSTR)(LPCTSTR)csRK4;
	RKS[0]=strtoul(RKS1,NULL,16);
	RKS[1]=strtoul(RKS2,NULL,16);
	RKS[2]=strtoul(RKS3,NULL,16);
	RKS[3]=strtoul(RKS4,NULL,16);



	UpdateData(false);
}
