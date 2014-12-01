#pragma once
#include <WinSock2.h>
#include "type.h"

#define Tag2ReaderX 1
#define Tag2ReaderS 2
#define Reader2TagX 3
#define Reader2TagS 4
#define ExX	5
#define ExS 6
#define SecFunErr 15

#define SM7
#ifdef SM7
#define CryptLen	8
#define CryptKeyLen	16
#endif

class Auth
{
public:
	u32 TID[2];
	u16 THandle;
	u16 RNr;
	u16 RNt;
	u16 AKX;
	u32 RKS[4];
	u32 AKS[4];
	u32 SK[4];
	bool AKSgeted;
	SecPara secpara;
//	HWND hParentWnd;
	SOCKET ComSock;
	char RecvResponse[100];

	//安全参数获取命令
	void Get_SecPara();
	//选择鉴别类型
	void selectAuthMode();

protected:
	bool TagPassed;
	//标签对读写器异或鉴别
	void fnTag2ReaderX();
	//标签对读写器对称鉴别
	void fnTag2ReaderS();
	//读写器对标签异或鉴别
	void fnReader2TagX();
	//读写器对标签对称鉴别
	void fnReader2TagS();
	//双向异或鉴别
	void fnExX();
	//双向对称鉴别
	void fnExS();
	//设置安全参数
	void SetSecPara(u16 * secpa);
	//请求异或鉴别命令
	void	Req_XAuth();
	//异或鉴别命令
	void	XAuth();
	//单向异或鉴别命令
	void	Get_XAuth();
	//双向异或鉴别命令
	void	Req_XAuth_EX();
	//请求鉴别命令
	void	Req_SAuth();
	//鉴别命令
	void	SAuth();
	//单向鉴别命令
	void	Get_SAuth();
	//双向鉴别命令
	void	Mul_Auth();
	//等待RDP
	void	waitRDP();
};




#define CRC				0xffff
#define On				0x5555

#define CGet_SecPara	0xae
#define CReq_XAuth		0xb5
#define CXAuth			0xb6
#define CGet_XAuth		0xb7
#define CReq_XAuth_Ex	0xb8
#define CReq_SAuth		0xa0
#define CSAuth			0xb3
#define CGet_SAuth		0xac
#define CMul_Auth		0xa1
