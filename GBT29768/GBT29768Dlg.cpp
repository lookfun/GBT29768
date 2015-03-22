
// GBT29768Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GBT29768.h"
#include "GBT29768Dlg.h"
#include "afxdialogex.h"
#include "SOCK.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGBT29768Dlg 对话框


DWORD WINAPI ComThread(LPVOID lpParameter)
{
	HWND hWnd=(HWND)lpParameter;
	SendMessage(hWnd,WM_ListShow,(WPARAM)IDS_Connecting,(LPARAM)0);
	auth.ComSock=ComStart();
	SendMessage(hWnd,WM_ListShow,IDS_Connected,0);
	return 0;
}
DWORD WINAPI AuthThread(LPVOID lpParameter)
{
	auth.Get_SecPara();	
	auth.selectAuthMode();
	return 0;
}

CGBT29768Dlg::CGBT29768Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGBT29768Dlg::IDD, pParent)
	, csRK1(_T(""))
	, csRK2(_T(""))
	, csRK3(_T(""))
	, csRK4(_T(""))
	, RKorAK(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGBT29768Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_List);
	DDX_Text(pDX, IDC_EDITAKX, m_AKX);
	DDX_Text(pDX, IDC_EDITRK1, csRK1);
	DDX_Text(pDX, IDC_EDITRK2, csRK2);
	DDX_Text(pDX, IDC_EDITRK3, csRK3);
	DDX_Text(pDX, IDC_EDITRK4, csRK4);
	DDX_Radio(pDX, IDC_RADIO1, RKorAK);
}

BEGIN_MESSAGE_MAP(CGBT29768Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ListShow,&CGBT29768Dlg::OnListShow)
	ON_BN_CLICKED(IDC_Connect, &CGBT29768Dlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_AuthStart, &CGBT29768Dlg::OnBnClickedAuthstart)
END_MESSAGE_MAP()


// CGBT29768Dlg 消息处理程序

BOOL CGBT29768Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	hParentWnd=m_hWnd;
	m_AKX=0;
	csRK1.Format("11223344");
	csRK2.Format("55667788");
	csRK3.Format("9900aabb");
	csRK4.Format("ccddeeff");
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGBT29768Dlg::OnPaint()
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
HCURSOR CGBT29768Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

afx_msg LRESULT CGBT29768Dlg::OnListShow(WPARAM wParam,LPARAM lParam)
{
	CString showstring;
	if (lParam==0)
	{
	showstring.LoadString(wParam);
	}
	else
	{
		char *cstr=new char[2*lParam+1];
		cstr[2*lParam]=0;
		char *head=cstr;
		while(lParam>1)
		{
		showstring.AppendFormat("%02x",((char *)wParam)[0]&0xff);
		lParam--;
		wParam++;
		}
		
		while(lParam>1)
		{
			sprintf(cstr,"%02x",((char *)wParam)[0]&0xff);
			cstr+=2;
			wParam++;
			lParam--;
		}

	}
	m_List.InsertString(-1,showstring);
	return 1;
	/*
	switch (wParam)
	{
	case IDS_Connecting:
	showstring.LoadString(IDS_Connecting);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_Connected:
	showstring.LoadString(IDS_Connected);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_SendGetSP:
	showstring.LoadString(IDS_SendGetSP);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_recvReGetSP:
	showstring.LoadString(IDS_recvReGetSP);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_SPGetted:
	showstring.LoadString(IDS_SPGetted);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_TIDGetted:
	showstring.LoadString(IDS_TIDGetted);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_AuthSucceed:
	showstring.LoadString(IDS_AuthSucceed);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_AuthFailed:
	showstring.LoadString(IDS_AuthFailed);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	case IDS_Error:
	showstring.LoadString(IDS_Error);
	m_List.InsertString(-1,showstring);
	return 1;
	break;
	default:
	return 0;
	break;
	}
	*/
}

void CGBT29768Dlg::OnBnClickedConnect()
{

	HANDLE hthread1;
	hthread1=CreateThread(NULL,0,ComThread,hParentWnd,0,NULL);
	CloseHandle(hthread1);
}


void CGBT29768Dlg::OnBnClickedAuthstart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	auth.AKX=m_AKX&0xffff;
	if (RKorAK==0)
	{
		char * RKS1=(LPSTR)(LPCTSTR)csRK1;
		char * RKS2=(LPSTR)(LPCTSTR)csRK2;
		char * RKS3=(LPSTR)(LPCTSTR)csRK3;
		char * RKS4=(LPSTR)(LPCTSTR)csRK4;
		auth.RKS[0]=strtoul(RKS1,NULL,16);
		auth.RKS[1]=strtoul(RKS2,NULL,16);
		auth.RKS[2]=strtoul(RKS3,NULL,16);
		auth.RKS[3]=strtoul(RKS4,NULL,16);
		auth.AKSgeted=false;
	}
	else
	{
		char *AKS1=(LPSTR)(LPCTSTR)csRK1;
		char *AKS2=(LPSTR)(LPCTSTR)csRK2;
		char *AKS3=(LPSTR)(LPCTSTR)csRK3;
		char *AKS4=(LPSTR)(LPCTSTR)csRK4;
		/*
		char * AKS1=csRK1.GetBuffer(csRK1.GetLength());
		char * AKS2=csRK2.GetBuffer(csRK2.GetLength());
		char * AKS3=csRK3.GetBuffer(csRK3.GetLength());
		char * AKS4=csRK4.GetBuffer(csRK4.GetLength());
		
		csRK1.ReleaseBuffer();
		*/
		auth.AKS[0]=strtoul(AKS1,NULL,16);
		auth.AKS[1]=strtoul(AKS2,NULL,16);
		auth.AKS[2]=strtoul(AKS3,NULL,16);
		auth.AKS[3]=strtoul(AKS4,NULL,16);
		auth.AKSgeted=true;
	}

	HANDLE hthread2;
	hthread2=CreateThread(NULL,0,AuthThread,NULL,0,NULL);
	CloseHandle(hthread2);
}
