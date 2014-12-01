typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
struct SecPara
{
	u16 RK;
	u8 EncryptMode;
	u16 AKlen;
	u8 SecFun;
	bool AuthEN;
	bool SecComEN;
	bool CanAuth;
};