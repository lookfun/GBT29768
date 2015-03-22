#include "stdafx.h"
#include "GBT29768.h"
#include "Auth.h"
#include <iostream>
#include "resource.h"
#include "DecEnc.h"

#define ListData(A,B)		SendMessage(hParentWnd,WM_ListShow,(A),B)
#define	sendcommand		send(ComSock,(char *)&command,sizeof(command),0)

#define ListMessage(A)	SendMessage(hParentWnd,WM_ListShow,A,0)
#define Re				recv(ComSock,RecvResponse,100,0)
#define ShiftL(A,B)		(A<<B)|(A>>(16-B))

int BitCount(unsigned int n)
{
	unsigned int c =0 ;
	for (c =0; n; ++c)
		n &= (n -1) ; // 清除最低位的1
	return c ;
}
int Rand16()
{
	int temp;
	srand(rand()+clock());
	temp=int((65535)*rand()/(RAND_MAX + 1.0));
	return temp;
}
void u32tochar(char * out,u32 * in,int len)
{
	for (int i=0;i<len;i++)
	{
		out[i*4+0]=(in[i]>>24)&0xff;
		out[i*4+1]=(in[i]>>16)&0xff;
		out[i*4+2]=(in[i]>>8)&0xff;
		out[i*4+3]=(in[i])&0xff;
	}
}

void Auth::Get_SecPara()
{
	u8 command[6];
	sprintf((char *)&command,"%c%c%c%c%c",CGet_SecPara,THandle>>8,THandle&0xff,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_SendGetSP);
	ListData((unsigned)(&command),sizeof(command));
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	u16 secpa[3];
	secpa[0]=RecvResponse[0]*0x100+RecvResponse[1];
	secpa[1]=RecvResponse[2]*0x100+RecvResponse[3];
	secpa[2]=RecvResponse[4]*0x100+RecvResponse[5];
	SetSecPara(secpa);
	ListMessage(IDS_SPGetted);
	TID[0]=RecvResponse[6];
	TID[0]=TID[0]<<8+RecvResponse[7];
	TID[0]=TID[0]<<8+RecvResponse[8];
	TID[0]=TID[0]<<8+RecvResponse[9];
	TID[0]=TID[0]<<8+RecvResponse[10];
	TID[0]=TID[0]<<8+RecvResponse[11];
	TID[0]=TID[0]<<8+RecvResponse[12];
	TID[0]=TID[0]<<8+RecvResponse[13];
	TID[1]=RecvResponse[14];
	TID[1]=TID[1]<<8+RecvResponse[15];
	TID[1]=TID[1]<<8+RecvResponse[16];
	TID[1]=TID[1]<<8+RecvResponse[17];
	TID[1]=TID[1]<<8+RecvResponse[18];
	TID[1]=TID[1]<<8+RecvResponse[19];
	TID[1]=TID[1]<<8+RecvResponse[20];
	TID[1]=TID[1]<<8+RecvResponse[21];
	ListMessage(IDS_TIDGetted);

	if (!AKSgeted)
	{
		//AKS=
		AKSgeted=true;
	}
}
void Auth::selectAuthMode()
{
	switch (secpara.SecFun)
	{
	case Tag2ReaderX:
		fnTag2ReaderX();
		break;
	case Tag2ReaderS:
		fnTag2ReaderS();
		break;
	case Reader2TagX:
		fnReader2TagX();
		break;
	case Reader2TagS:
		fnReader2TagS();
		break;
	case ExX:
		fnExX();
		break;
	case ExS:
		fnExS();
		break;
	default:
		break;
	}
	ListMessage(IDS_Finished);
}
void Auth::fnTag2ReaderX()
{
	Req_XAuth();
	XAuth();
	waitRDP();
}
void Auth::fnTag2ReaderS()
{
	Req_SAuth();
	SAuth();
	waitRDP();
}
void Auth::fnReader2TagX()
{
	TagPassed=false;
	Get_XAuth();
}
void Auth::fnReader2TagS()
{
	Get_SAuth();
}
void Auth::fnExX()
{
	TagPassed=false;
	Req_XAuth_EX();
	if (TagPassed)
	{
		XAuth();
		waitRDP();
	}
}
void Auth::fnExS()
{
	Req_SAuth();
	Mul_Auth();
}
void Auth::Req_XAuth()
{
	u8 command[6];
	sprintf((char *)&command,"%c%c%c%c%c",CReq_XAuth,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_Req_XAuth);
	ListData((unsigned)(&command),sizeof(command));
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	u16 SRNt;
	SRNt=(RecvResponse[0]<<8&0xff00)|(RecvResponse[1]&0xff);
	//RNt=(SRNt^AKX+0xAAAB)&0xffff;
	RNt=(SRNt^AKX)>0x5555? ((SRNt^AKX)-(int)0x5555):(SRNt^AKX+0xAAAB);
}
void Auth::XAuth()
{
	int p=BitCount(RNt);
	u16 RNtp=ShiftL(RNt,p);//(RNt<<p)|(RNt>>(16-p));
	u16 AKXp=(AKX<<p)|(AKX>>(16-p));
	u16 SORNt=RNtp^(AKXp+On)&0xffff;
	char command[8];
	sprintf(command,"%c%c%c%c%c%c%c",CXAuth,SORNt>>8,SORNt,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_XAuth);
	ListData((unsigned)(&command),sizeof(command));
}
void Auth::Get_XAuth()
{
	char command[8];
	RNr=Rand16();
	u16 SRNr=((RNr+On)&0xffff)^AKX;
	sprintf(command,"%c%c%c%c%c%c%c",CGet_XAuth,SRNr>>8,SRNr,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_Get_XAuth);
	ListData((unsigned)(&command),sizeof(command));

	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	u16 SORNr=(RecvResponse[0]<<8&0xff00)|(RecvResponse[1]&0xff);
	int p=BitCount(RNr);
	u16 RNrp=ShiftL(RNr,p);
	u16 AKXp=ShiftL(AKX,p);
	if ((SORNr^RNrp)==((AKXp+On)&0xffff))
	{
		ListMessage(IDS_TagAuthSucceed);
	} 
	else
	{
		ListMessage(IDS_TagAuthFailed);
	}
}
void Auth::Req_XAuth_EX()
{
	char command[8];
	RNr=Rand16();
	u16 SRNr=((RNr+On)&0xffff)^AKX;
	sprintf(command,"%c%c%c%c%c%c%c",CReq_XAuth_Ex,SRNr>>8,SRNr,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_Req_XAuth_EX);
	ListData((unsigned)(&command),sizeof(command));
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	u16 SORNr=(RecvResponse[0]<<8&0xff00)|(RecvResponse[1]&0xff);
	u16 SRNt=(RecvResponse[2]<<8&0xff00)|(RecvResponse[3]&0xff);
	int p=BitCount(RNr);
	u16 RNrp=ShiftL(RNr,p);
	u16 AKXp=ShiftL(AKX,p);
	if ((SORNr^RNrp)==((AKXp+On)&0xffff))
	{
		TagPassed=true;
		ListMessage(IDS_TagAuthSucceed);
	}
	else
	{
		TagPassed=false;
		ListMessage(IDS_TagAuthFailed);
	}
	RNt=(SRNt^AKX)>0x5555? ((SRNt^AKX)-(int)0x5555):(SRNt^AKX+0xAAAB);
}
void Auth::Req_SAuth()
{
	char command[6];
	sprintf(command,"%c%c%c%c%c",CReq_SAuth,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_Req_SAuth);
	ListData((unsigned)(&command),sizeof(command));
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	RNt=(RecvResponse[0]<<8&0xff00)|(RecvResponse[1]&0xff);
}
void Auth::SAuth()
{
	char command[16];
	char In[CryptLen];
	char Out[CryptLen+1];
	char Out2[CryptLen];
	int Outlen;
	memset(In,0,CryptLen);
	memset(Out,0,CryptLen+1);
	In[0]=RNt>>8&0xff;
	In[1]=RNt&0xff;
	char cAKS[16];
	u32tochar(cAKS,AKS,4);
	Enc((unsigned char *)&In,8,(unsigned char *)&Out,&Outlen,(unsigned char *)&cAKS,16);
	//	Enc((unsigned char *)&Out,8,(unsigned char *)&Out2,&Outlen,(unsigned char *)&cAKS,16);
	sprintf(command,"%c%c%c%s%c%c%c%c",CSAuth,RNt>>8,RNt,Out,THandle>>8,THandle,CRC>>8,CRC);
	//	sprintf((char *)&command[11],"%c%c%c%c",THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	//	send(ComSock,command,15,0);
	ListMessage(IDS_SAuth);
	ListData((unsigned)(&command),sizeof(command));
}
void Auth::Get_SAuth()
{
	char command[8];
	RNr=Rand16();
	sprintf(command,"%c%c%c%c%c%c%c",CGet_SAuth,RNr>>8,RNr,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_Get_SAuth);
	ListData((unsigned)(&command),sizeof(command));
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	u16 rRNr=(RecvResponse[0]<<8&0xff00)|(RecvResponse[1]);
	if (rRNr==RNr)
	{
		char Out[8];
		char cAKS[16];
		u32tochar(cAKS,AKS,4);
		int outlen;
		Dec((unsigned char *)&RecvResponse[2],8,(unsigned char *)&Out,&outlen,(unsigned char *)&cAKS,16);
		rRNr=(((int)Out[0]<<8)&0xff00)|((int)Out[1]&0xff);
		if (rRNr==RNr)
			ListMessage(IDS_TagAuthSucceed); 
		else
			ListMessage(IDS_TagAuthFailed);
	} 
	else
		ListMessage(IDS_TagAuthFailed);
}
void Auth::Mul_Auth()
{
	char command[8+CryptLen];
	RNr=Rand16();
	char In[8];
	char Out[9];
	char cAKS[16];
	u32tochar(cAKS,AKS,4);
	int outlen;
	memset(In,0,8);
	memset(Out,0,9);

	sprintf(In,"%c%c%c%c",RNr>>8,RNr,RNt>>8,RNt);
	Enc((unsigned char *)&In,8,(unsigned char *)&Out,&outlen,(unsigned char *)&cAKS,16);
	sprintf(command,"%c%c%c%s%c%c%c%c",CMul_Auth,RNt>>8,RNt,Out,THandle>>8,THandle,CRC>>8,CRC);
	sendcommand;

	ListMessage(IDS_Mul_Auth);
	ListData((unsigned)(&command),sizeof(command));
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	if (recvlen==6)
	{
		switch (RecvResponse[0]&0xff)
		{
		case 0x00:
			ListMessage(IDS_AuthSucceed);
			break;
		case 0x87:
			ListMessage(IDS_AuthFailed);
			break;
		default:
			ListMessage(IDS_Error);
			break;
		}
	} 
	else
	{
		ListMessage(IDS_AuthSucceed); 
		u16 rRNr=(RecvResponse[0]<<8&0xff00)|(RecvResponse[1]&0xff);
		if (rRNr==RNr)
			ListMessage(IDS_TagAuthSucceed);
		else
			ListMessage(IDS_TagAuthFailed);
	}
}
void Auth::SetSecPara(u16 * secpa)
{
	/*
	if (secpa[0]&0x8000)//first bit
	{
	if (secpa[0]&0x4000)//second bit
	{
	secpara.CanAuth=true;
	secpara.AuthEN=true;
	secpara.SecComEN=true;
	} 
	else
	{
	secpara.CanAuth=true;
	secpara.AuthEN=true;
	secpara.SecComEN=false;
	}
	} 
	else
	{
	if (secpa[0]&0x4000)
	{			
	secpara.CanAuth=true;
	secpara.AuthEN=false;
	secpara.SecComEN=false;

	} 
	else
	{
	secpara.CanAuth=false;
	secpara.AuthEN=false;
	secpara.SecComEN=false;
	}
	}
	*/
	secpara.CanAuth=secpa[0]&0xc000;
	secpara.AuthEN=secpa[0]&0x8000;
	secpara.SecComEN=(secpa[0]&0x8000)&(secpa[0]&0x4000);
	//安全功能
	/*
	switch ((secpa[0]&0x3f80)>>7)//3 to 9 bit
	{
	case 64:
	secpara.SecFun=1;
	break;
	case 32:
	secpara.SecFun=2;
	break;
	}
	*/
	int secfun=(secpa[0]&0x3f80)>>8;
	secpara.SecFun=7;
	while (secfun)
	{
		secpara.SecFun--;
		secfun=secfun>>1;
	}

	//加密算法
	secpara.EncryptMode=secpa[1]&0x0f00>>8;


	//密钥长度
	secpara.AKlen=(secpa[2]&0x00ff)*16;
	//RK
	secpara.RK=secpa[3];

}
void Auth::waitRDP()
{
	int recvlen=Re;
	ListData((unsigned)(&RecvResponse),recvlen);
	switch (RecvResponse[0]&0xff)
	{
	case 0x00:
		ListMessage(IDS_AuthSucceed);
		break;
	case 0x87:
		ListMessage(IDS_AuthFailed);
		break;
	default:
		ListMessage(IDS_Error);
		break;
	}
}
