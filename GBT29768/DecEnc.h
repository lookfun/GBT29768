
extern "C" int Crypt_Dec_Block(unsigned char *input,int in_len,unsigned char *output,int *out_len,unsigned char *key,int keylen);
extern "C" int Crypt_Enc_Block(unsigned char *input,int in_len,unsigned char *output,int *out_len,unsigned char *key,int keylen);

int Dec(unsigned char *input,int in_len,unsigned char *output,int *out_len,unsigned char *key,int keylen)
{
	return Crypt_Dec_Block(input,in_len,output,out_len,key,keylen);
}
int Enc(unsigned char *input,int in_len,unsigned char *output,int *out_len,unsigned char *key,int keylen)
{
	return Crypt_Enc_Block(input,in_len,output,out_len,key,keylen);
}