#include "stdafx.h"
#include "AuthTag.h"
#include "Tag.h"

extern "C" int Crypt_Dec_Block(unsigned char *input,int in_len,unsigned char *output,int *out_len,unsigned char *key,int keylen);
extern "C" int Crypt_Enc_Block(unsigned char *input,int in_len,unsigned char *output,int *out_len,unsigned char *key,int keylen);

#define	sendresponse	send(ComSock,(char *)&recommand,sizeof(recommand),0)
#define ListMessage(A) 	SendMessage(hParentWnd,WM_ListShow,A,0)
#define Re				recv(ComSock,RecvCommand,100,0)
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
	srand(rand());
	temp=int((65535)*rand()/(RAND_MAX + 1.0));
	return temp;
}
void u16tochar(char * out,u16 * in,int len)
{
	for (int i=0;i<len;i++)
	{
		out[i*2]=in[i]>>8;
		out[i*2+1]=in[i]&0xff;
	}
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


void AuthTag::response()
{
	int len=1;
	while(len>0)
	{
		len=Re;
		if (len>0)
			if(SwitchCommand()==1)
			{
				ListMessage(IDS_Finished);
				break;
			}
		else
		{//连接断开
		//	ListMessage(IDS_DisConnected);
		}
	}
}

int AuthTag::SwitchCommand()
{
	switch ((u8)RecvCommand[0])
	{
		//Get_SecPara	10101110
	case 174:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvGet_SecPara,0);
		reGet_SecPara();
		break;
		//Req_XAuth		10110101	
	case 181:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvReq_XAuth,0);
		reReq_XAuth();
		break;
		//XAuth			10110110
	case 182:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvXAuth,0);
		reXAuth();
		return 1;
		break;
		//Get_XAuth		10110111
	case 183:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvGet_XAuth,0);
		reGet_XAuth();
		return 1;
		break;
		//Req_XAuth_EX	10111000
	case 184:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvReq_XAuth_EX,0);
		reReq_XAuth_EX();
		break;
		//Req_SAuth		10100000
	case 160:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvReq_SAuth,0);
		reReq_SAuth();
		break;
		//SAuth			10110011
	case 179:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvSAuth,0);
		reSAuth();
		return 1;
		break;
		//Get_SAuth		10101100
	case 172:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvGet_SAuth,0);
		reGet_SAuth();
		return 1;
		break;
		//Mul_Auth		10100001
	case 161:
		SendMessage(hParentWnd,WM_ListShow,IDS_recvMul_Auth,0);
		reMul_Auth();
		return 1;
		break;
	default:
		break;
	}
	return 0;
}
void AuthTag::reGet_SecPara()
{
	char recommand[19];
	SecParaTag[0]=(0x1<<(14-AuthMode))|0x8000;
	SecParaTag[1]=0x0700;
	SecParaTag[3]=RKindex;
	char cSecPara[6];
	char cTID[8];
	u16tochar(cSecPara,SecParaTag,3);
	u32tochar(cTID,TID,2);
	/*
	memcpy(recommand,cSecPara,6);
	memcpy(recommand+6,cTID,8);
	memcpy(recommand+14,&THandle,2);
	memcpy(recommand+16,,2);
	*/
	sprintf(recommand,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",cSecPara[0],cSecPara[1],cSecPara[2],cSecPara[3],cSecPara[4],cSecPara[5],\
		cTID[0],cTID[1],cTID[2],cTID[3],cTID[4],cTID[5],cTID[6],cTID[7],THandle>>8,THandle,CRC>>8,CRC);
	sendresponse;
	ListMessage(IDS_reGet_SecPara);
}
void AuthTag::reReq_XAuth()
{
	char recommand[7];
	RNt=Rand16();
	u16 SRNt;
	SRNt=((RNt+On)^AKX)&0xffff;
	sprintf(recommand,"%c%c%c%c%c%c",SRNt>>8,SRNt,THandle>>8,THandle,CRC>>8,CRC);
	sendresponse;
	ListMessage(IDS_reReq_XAuth);
}
void AuthTag::reXAuth()
{

	u16 SORNt;
	SORNt=(RecvCommand[1]<<8&0xff00)|(RecvCommand[2]&0xff);
	int p=BitCount(RNt);
	u16 RNtp=ShiftL(RNt,p);
	u16 AKXp=ShiftL(AKX,p);
	if (((AKXp+On)&0xffff)==(SORNt^RNtp))
		RDPT();
	else
		RDPF();
	ListMessage(IDS_reXAuth);
}
void AuthTag::reGet_XAuth()
{
	char recommand[7];
	u16 SRNr=(RecvCommand[1]<<8&0xff00)|(RecvCommand[2]&0xff);
	RNr=(SRNr^AKX)>0x5555? ((SRNr^AKX)-(int)0x5555):(SRNr^AKX+0xAAAB);
	int p=BitCount(RNr);
	u16	RNrp=ShiftL(RNr,p);
	u16 AKXp=ShiftL(AKX,p);
	u16 SORNr=RNrp^((AKXp+On)&0xffff);
	sprintf(recommand,"%c%c%c%c%c%c",SORNr>>8,SORNr,THandle>>8,THandle,CRC>>8,CRC);
	sendresponse;
	ListMessage(IDS_reGet_XAuth);
}
//响应双向异或鉴别命令
void AuthTag::reReq_XAuth_EX()
{
	char recommand[9];
	u16 SRNr=(RecvCommand[1]<<8&0xff00)|(RecvCommand[2]&0xff);
	RNr=(SRNr^AKX)>0x5555? ((SRNr^AKX)-(int)0x5555):(SRNr^AKX+0xAAAB);
	int p=BitCount(RNr);
	u16	RNrp=ShiftL(RNr,p);
	u16 AKXp=ShiftL(AKX,p);
	u16 SORNr=RNrp^((AKXp+On)&0xffff);

	RNt=Rand16();
	u16 SRNt=((RNt+On)^AKX)&0xffff;

	sprintf(recommand,"%c%c%c%c%c%c%c%c",SORNr>>8,SORNr,SRNt>>8,SRNt,THandle>>8,THandle,CRC>>8,CRC);
	sendresponse;
	ListMessage(IDS_reReq_XAuth_EX);

}
//响应请求鉴别命令
void AuthTag::reReq_SAuth()
{
	char recommand[7];
	RNt=Rand16();
	sprintf(recommand,"%c%c%c%c%c%c",RNt>>8,RNt,THandle>>8,THandle,CRC>>8,CRC);
	sendresponse;
	ListMessage(IDS_reReq_SAuth);

}
//响应鉴别命令
void AuthTag::reSAuth()
{
	u16 rRNt;
	rRNt=(RecvCommand[1]<<8&0xff00)|(RecvCommand[2]&0xff);
	if (rRNt==RNt)
	{
		unsigned char In[8];
		unsigned char Out[8];
		int outlen;
		char cAKS[16];
		u32tochar(cAKS,AKS,4);
		Crypt_Dec_Block((unsigned char *)&RecvCommand[3],8,Out,&outlen,(unsigned char *)&cAKS,16);
		rRNt=(((int)Out[0]<<8)&0xff00)|((int)Out[1]&0xff);
		if (rRNt==RNt)
			RDPT(); 
		else
			RDPF();
	} 
	else
		RDPF();
	ListMessage(IDS_reSAuth);
}
//响应单向鉴别命令
void AuthTag::reGet_SAuth()
{
	char recommand[CryptLen+7];
	char In[8];
	char Out[9];
	int outlen;
	char cAKS[16];
	memset(In,0,8);
	memset(Out,0,9);
	u32tochar(cAKS,AKS,4);
	RNr=(RecvCommand[1]<<8&0xff00)|(RecvCommand[2]&0xff);
	In[0]=RecvCommand[1];
	In[1]=RecvCommand[2];
	Crypt_Enc_Block((unsigned char *)&In,8,(unsigned char *)&Out,&outlen,(unsigned char *)&cAKS,16);
	sprintf(recommand,"%c%c%s%c%c%c%c",RNr>>8,RNr,Out,THandle>>8,THandle,CRC>>8,CRC);
	sendresponse;
	ListMessage(IDS_reGet_SAuth);
}
//响应双向鉴别命令
void AuthTag::reMul_Auth()
{
	u16 rRNt=(RecvCommand[1]<<8&0xff00)|(RecvCommand[2]&0xff);
	if (rRNt==RNt)
	{
		char Out[8];
		char cAKS[16];
		int outlen;
		memset(Out,0,8);
		u32tochar(cAKS,AKS,4);
		Crypt_Dec_Block((unsigned char *)&RecvCommand[3],8,(unsigned char *)&Out,&outlen,(unsigned char *)&cAKS,16);
		RNr=(Out[0]<<8&0xff00)|(Out[1]&0xff);
		rRNt=(Out[2]<<8&0xff00)|(Out[3]&0xff);
		if (rRNt==RNt)
		{
			ListMessage(IDS_AuthSucceed);
			char recommand[7];
			sprintf(recommand,"%c%c%c%c%c%c",RNr>>8,RNr,THandle>>8,THandle,CRC>>8,CRC);
			sendresponse;
			ListMessage(IDS_reMul_Auth);
		} 
		else
			RDPF();
	}
	else
		RDPF();
}
void AuthTag::RDPF()
{
	char recommand[6];
	sprintf(recommand,"%c%c%c%c%c",0x87,THandle>>8,THandle,CRC>>8,CRC);
	ListMessage(IDS_AuthFailed);
	sendresponse;
}
void AuthTag::RDPT()
{
	char recommand[6];
	sprintf(recommand,"%c%c%c%c%c",0x00,THandle>>8,THandle,CRC>>8,CRC);
	ListMessage(IDS_AuthSucceed);
	sendresponse;
}