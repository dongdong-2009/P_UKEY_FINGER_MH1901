#ifndef __APP_CMD_H_
#define __APP_CMD_H_


#pragma pack(1)/*指定按1字节对齐*/

typedef enum{	
	EF_NORMAL=0,
	RSA_PUB=0x0A,
	RSA_PRI=0x0B,
	SM2_PUB=0x0D,
	SM2_PRI=0x0E,
	CERT_FILE=0x0C,
	PRINT_FILE=0X0F
}APP_FILE_TYPE;


#define KEY_CMD_DEVAUTH  0x0050

typedef struct
{
	unsigned short cmd;
	unsigned short (*function)();
}CMD_TABLE;


typedef struct
{
	unsigned char type;
	unsigned char key_sta;
	unsigned char iv_sta;
	unsigned char rfu;
	unsigned char key[16];
	unsigned char iv[16];
	unsigned char init;
}CRYPT_DATA;


typedef struct
{	
	unsigned char state;
	unsigned char type;
	unsigned char keyid[2];//会话秘钥ID	
	unsigned char key[16];
}SESSION_KEY;


typedef struct _apdu{
	unsigned char rfu[2];
	unsigned char CLA;
	unsigned char INS;
	unsigned char PP1;
	unsigned char PP2;
	unsigned char LEN[2];
//	unsigned char pInData[1024];
//	unsigned char pOutData[1024];
	unsigned char *pInData;
	unsigned char *pOutData;
	unsigned char SW1;
	unsigned char SW2;
	unsigned int LC;
	unsigned int  RLE;
}APDU;


extern APDU g_APDU;

#pragma pack () /*取消指定对齐，恢复缺省对齐*/


#endif

