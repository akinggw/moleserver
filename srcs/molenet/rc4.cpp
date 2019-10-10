#include <stdio.h>
#include <string.h>
#include "time.h"

#include "../../include/molnet/rc4.h"


int Rc4Encrypt(const char* szPassWord,unsigned char* szSource,unsigned char * szDec,unsigned short len)
{
	int ret_len = 0;

	if(szSource == NULL || szPassWord == NULL || szDec == NULL || len <= 0) return ret_len;

	RC4(szSource,
			len,
			(unsigned char*)szPassWord,
			(int)strlen(szPassWord),
			szDec,
			&ret_len);

	return ret_len;
}

int Rc4Decrypt(const char* szPassWord,unsigned char* szSource,unsigned char * szDec,unsigned short len)
{
	int ret_len = 0;

	if(szSource == NULL || szPassWord == NULL || szDec == NULL || len <= 0) return ret_len;

	RC4(szSource,
		len,
		(unsigned char*)szPassWord,
		(int)strlen(szPassWord),
		szDec,
		&ret_len);

	return ret_len;
}

int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len)
{
	if (data == NULL || key == NULL || out == NULL)
		return 0;

	unsigned char mBox[BOX_LEN];

	if(GetKey(key, key_len, mBox) == NULL)
		return 0;

	int i=0;
	int x=0;
	int y=0;

	for(int k = 0; k < data_len; k++)
	{
		x = (x + 1) % BOX_LEN;
		y = (mBox[x] + y) % BOX_LEN;
		swap_byte(&mBox[x], &mBox[y]);
		out[k] = data[k] ^ mBox[(mBox[x] + mBox[y]) % BOX_LEN];
	}

	*out_len = data_len;

	return -1;
}

int GetKey(const unsigned char* pass, int pass_len, unsigned char* out)
{
	if(pass == NULL || out == NULL)
		return 0;

	int i;

	for(i = 0; i < BOX_LEN; i++)
		out[i] = i;

	int j = 0;
	for(i = 0; i < BOX_LEN; i++)
	{
		j = (pass[i % pass_len] + out[i] + j) % BOX_LEN;
		swap_byte(&out[i], &out[j]);
	}

	return -1;
}

static void swap_byte(unsigned char* a, unsigned char* b)
{
	unsigned char swapByte;

	swapByte = *a;
	*a = *b;
	*b = swapByte;
}

//CheckSum:计算校验和的子函数
unsigned short checksum(unsigned short *buffer, int size)
{
	unsigned long cksum=0;
	while(size >1)
	{
		cksum+=*buffer++;
		size -=sizeof(unsigned short);
	}
	if(size )
	{
		cksum += *(unsigned short*)buffer;
	}

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (unsigned short)(~cksum);
}
