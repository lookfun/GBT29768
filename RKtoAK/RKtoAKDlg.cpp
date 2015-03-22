
// RKtoAKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RKtoAK.h"
#include "RKtoAKDlg.h"
#include "afxdialogex.h"
#include "SMS4DLLextern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRKtoAKDlg 对话框



CRKtoAKDlg::CRKtoAKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRKtoAKDlg::IDD, pParent)
	, csRK1(_T(""))
	, csRK2(_T(""))
	, csRK3(_T(""))
	, csRK4(_T(""))
	, csTID1(_T(""))
	, csTID2(_T(""))
	, csAK1(_T(""))
	, csAK2(_T(""))
	, csAK3(_T(""))
	, csAK4(_T(""))
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
	DDX_Text(pDX, IDC_EDIT9, csTID1);
	DDX_Text(pDX, IDC_EDIT10, csTID2);
	DDX_Text(pDX, IDC_EDIT5, csAK1);
	DDX_Text(pDX, IDC_EDIT6, csAK2);
	DDX_Text(pDX, IDC_EDIT7, csAK3);
	DDX_Text(pDX, IDC_EDIT8, csAK4);
}

BEGIN_MESSAGE_MAP(CRKtoAKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRKtoAKDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRKtoAKDlg::OnBnClickedButton2)
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
	unsigned int TID[4];
	char *RKS1=(LPSTR)(LPCTSTR)csRK1;
	char *RKS2=(LPSTR)(LPCTSTR)csRK2;
	char *RKS3=(LPSTR)(LPCTSTR)csRK3;
	char *RKS4=(LPSTR)(LPCTSTR)csRK4;
	char *TID1=(LPSTR)(LPCTSTR)csTID1;
	char *TID2=(LPSTR)(LPCTSTR)csTID2;
	RKS[0]=strtoul(RKS1,NULL,16);
	RKS[1]=strtoul(RKS2,NULL,16);
	RKS[2]=strtoul(RKS3,NULL,16);
	RKS[3]=strtoul(RKS4,NULL,16);
	TID[0]=strtoul(TID1,NULL,16);
	TID[1]=strtoul(TID2,NULL,16);
	TID[2]=0;
	TID[3]=0;
	sms4_encrypt(&TID[0],&RKS[0]);
	csAK1.Format(_T("%08x"),TID[0]);
	csAK2.Format(_T("%08x"),TID[1]);
	csAK3.Format(_T("%08x"),TID[2]);
	csAK4.Format(_T("%08x"),TID[3]);
	UpdateData(false);
}


void CRKtoAKDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned int RKS[4];
	unsigned int TID[4];
	char *RKS1=(LPSTR)(LPCTSTR)csRK1;
	char *RKS2=(LPSTR)(LPCTSTR)csRK2;
	char *RKS3=(LPSTR)(LPCTSTR)csRK3;
	char *RKS4=(LPSTR)(LPCTSTR)csRK4;
	RKS[0]=strtoul(RKS1,NULL,16);
	RKS[1]=strtoul(RKS2,NULL,16);
	RKS[2]=strtoul(RKS3,NULL,16);
	RKS[3]=strtoul(RKS4,NULL,16);
	char *AK1=(LPSTR)(LPCTSTR)csAK1;
	char *AK2=(LPSTR)(LPCTSTR)csAK2;
	char *AK3=(LPSTR)(LPCTSTR)csAK3;
	char *AK4=(LPSTR)(LPCTSTR)csAK4;
	unsigned int AK[4];
	AK[0]=strtoul(AK1,NULL,16);
	AK[1]=strtoul(AK2,NULL,16);
	AK[2]=strtoul(AK3,NULL,16);
	AK[3]=strtoul(AK4,NULL,16);
	sms4_decrypt(&AK[0],&RKS[0]);
	
	csAK1.Format(_T("%08x"),AK[0]);
	csAK2.Format(_T("%08x"),AK[1]);
	csAK3.Format(_T("%08x"),AK[2]);
	csAK4.Format(_T("%08x"),AK[3]);
	UpdateData(false);
}
