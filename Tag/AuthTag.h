#pragma once
#include <WinSock2.h>
#include "type.h"

#define SM7
#ifdef SM7
#define CryptLen	8
#define CryptKeyLen	16
#endif

class AuthTag
{
public:
	u32 TID[2];
	u16 RNt;
	u16 RNr;
	u16 THandle;
	u16 RKindex;
	u16 AKX;
	u32 AKS[4];
	u8 AuthMode;
	SOCKET ComSock;
	u16 SecParaTag[3];
	void response();
protected:
//	char recommand[100];
	char RecvCommand[100];
	int SwitchCommand();
	//响应安全参数获取
	void reGet_SecPara();
	//响应请求异或鉴别命令
	void reReq_XAuth();
	//响应异或鉴别命令
	void reXAuth();
	//响应单向异或鉴别命令
	void reGet_XAuth();
	//响应双向异或鉴别命令
	void reReq_XAuth_EX();
	//响应请求鉴别命令
	void reReq_SAuth();
	//响应鉴别命令
	void reSAuth();
	//响应单向鉴别命令
	void reGet_SAuth();
	//响应双向鉴别命令
	void reMul_Auth();
	void RDPF();
	void RDPT();
};
#define CRC				0xffff
#define On				0x5555