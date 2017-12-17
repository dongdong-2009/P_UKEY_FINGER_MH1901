#include "hal_alg.h"
#include "cgu.h"
#include "sm1.h"
#include "ssf33.h"
#include "sms4.h"
#include "hash.h"
#include "reae.h"
#include "ecc.h"
#include "ecp.h"
#include "arithm.h"
void TOOL_BcdToAsc(unsigned char *Dest,unsigned char *Src,unsigned int Len)
{
    unsigned int i;
	for(i=0;i<Len;i++)
	{
	    //高Nibble转换
	    if(((*(Src + i) & 0xF0) >> 4) <= 9)
	    {
            *(Dest + 2*i) = ((*(Src + i) & 0xF0) >> 4) + 0x30;
        }
        else
        {
            *(Dest + 2*i)  = ((*(Src + i) & 0xF0) >> 4) + 0x37;   //大写A~F
        }    
        //低Nibble转换
        if((*(Src + i) & 0x0F) <= 9)
        {
            *(Dest + 2*i + 1) = (*(Src + i) & 0x0F) + 0x30;
        }
        else
        {
            *(Dest + 2*i + 1) = (*(Src + i) & 0x0F) + 0x37;   //大写A~F
        }    
    }
}
//填充数据
void hal_alg_filldata(unsigned char *pbin,unsigned char *pbout,unsigned char per_len,unsigned short *len)
{
	unsigned short tmp_len=*len;
	unsigned char len1;
	if(pbin!=pbout)
		memmove(pbout,pbin,tmp_len);
	pbout[tmp_len++]=0x80;
	len1=tmp_len%per_len;
	if(len1)
	{
		memset(pbout+tmp_len,0x00,per_len-len1);
	}
	tmp_len+=per_len-len1;
	*len=tmp_len;
}
//去填充数据,两个字节的长度+数据+填充
unsigned char hal_alg_defilldata(unsigned char *pbin,unsigned short *len)
{
	unsigned short len1,tmp_len=*len;
	len1=(pbin[0]<<8)+pbin[1];
	while(tmp_len>0)
	{
		if(pbin[tmp_len--]==0x80)
			break;
	}
	tmp_len--;//减去80一字节长度
	
	if(len1==tmp_len)
	{
		*len=tmp_len;
		memmove(pbin,pbin+2,len1);
		return 0;
	}
	else 
		return 1;
}

void hal_alg_genrand(unsigned char *buf, unsigned short len)
{
	unsigned char loop = len / 4;
	unsigned char i;

	unsigned int tmp[64];

	TRNG_Init();
	
	if(0 != len % 4)
		loop++;
	
	for(i=0; i<loop; i++)
		tmp[i] = Get_Rand();

	memcpy(buf, tmp, len);
	
}

unsigned char hal_3des_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
	unsigned char cbc,encrypt;

	unsigned int *piIn = 0, *piOut=0;
	unsigned int piIv[2],piKey[6],pTmp[16];
	
	piIn = piOut = pTmp;

	MCSCR |= DES_ON;
	
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}

	if(cbc)
		memcpy((unsigned char*)piIv, piv, 8);
		
	memcpy((unsigned char*)piIn, pbin, 16);
	memcpy((unsigned char*)piKey, pkey, 16);

	DES_CRYPT(piIn, 16, piKey, 16, piIv, encrypt, cbc, piOut);
	memcpy(pbout, piOut, 16);
	
//	UART_printf("3des00\r\n");
//	DES_CRYPT((U32*)pbin, len, (U32*)pkey, 16, (U32*)piv, encrypt, cbc, (U32*)pbout);
//	UART_printf("3des01\r\n");

	MCSCR &= ~DES_ON;
	return 0;
}
unsigned char hal_sm1_init(		unsigned char *pkey,
								unsigned char *piv,
								unsigned char type)
{
	unsigned int csr;
	if((SYMALG_MODEIS & type) == SYMALG_ECB&&(SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		csr=SM1_128_ECB_ENCRYPT;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC&&(SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		csr=SM1_128_CBC_ENCRYPT;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_ECB&&(SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		csr=SM1_128_ECB_DECRYPT;
	}
	 if((SYMALG_MODEIS & type) == SYMALG_CBC&&(SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		csr=SM1_128_CBC_DECRYPT;
//		SM1_Init();
		SM1_Setup(csr, (U32*)pkey,(U32*)piv);

		return 0;
	}
	
//	SM1_Init();
	SM1_Setup(csr, (U32*)pkey,(U32*)piv);

	return 0;
}
unsigned char hal_sm1_cryp(unsigned char *pbin,
								unsigned int len,
								unsigned char *pbout,
								unsigned char flag)
{
	if(flag==1)
	{
		SM1_Run_CBC_Decrypt((U32*)pbin,(U32*)pbout, len/16);
	}
	else
	{
		SM1_Run((U32*)pbin,(U32*)pbout, len/16);	
	}

	return 0;
}
unsigned char hal_sm1_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
#if 0
	unsigned char cbc,encrypt;
	static unsigned char flag=0;
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}
	
//	MCSCR |= SM1_ON;
//	SM1_Init();
	if(flag==0)
	{
		SM1_CRYPT(pbin, len, pkey, NEW_KEY, piv, encrypt, cbc, pbout);	
		flag=1;
	}
	else
		SM1_CRYPT(pbin, len, pkey, KEY_NOT_CHANGE, piv, encrypt, cbc, pbout);	
	
//	MCSCR &= ~SM1_ON;
#else
	unsigned int csr;
	if((SYMALG_MODEIS & type) == SYMALG_ECB&&(SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		csr=SM1_128_ECB_ENCRYPT;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC&&(SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		csr=SM1_128_CBC_ENCRYPT;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_ECB&&(SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		csr=SM1_128_ECB_DECRYPT;
	}
	 if((SYMALG_MODEIS & type) == SYMALG_CBC&&(SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		csr=SM1_128_CBC_DECRYPT;
		SM1_Setup(csr, (U32*)pkey,(U32*)piv);
		SM1_Run_CBC_Decrypt((U32*)pbin,(U32*)pbout, len/16);
		return 0;
	}
	
	
	SM1_Setup(csr, (U32*)pkey,(U32*)piv);
	SM1_Run((U32*)pbin,(U32*)pbout, len/16);	

#endif

	return 0;
}
unsigned char hal_ssf33_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
#if 0
	unsigned char cbc,encrypt;
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}
	
	MCSCR |= SSF33_ON;
	
	SSF33_CRYPT(pbin, len,(unsigned int*) pkey, piv, encrypt, cbc, pbout);

	MCSCR &= ~SSF33_ON;
#else
	unsigned int csr;
	unsigned int process_key[4];
	if((SYMALG_MODEIS & type) == SYMALG_ECB&&(SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		csr=SSF33_ECB_ENCRYPT;
		SSF33_ENCKEY((U32*)pkey,process_key);
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC&&(SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		csr=SSF33_CBC_ENCRYPT;
		SSF33_ENCKEY((U32 *)pkey,process_key);
	}
	else if((SYMALG_MODEIS & type) == SYMALG_ECB&&(SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		csr=SSF33_ECB_DECRYPT;
		SSF33_DECKEY((U32*)pkey,process_key);
	}
	 if((SYMALG_MODEIS & type) == SYMALG_CBC&&(SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		csr=SSF33_CBC_DECRYPT;
		SSF33_DECKEY((U32 *)pkey,process_key);
	}
	
	SSF33_Setup(csr, process_key, (U32*)piv);
	SSF33_Run((U32*)pbin, (U32*)pbout, len/16);

#endif

	return 0;
}
unsigned char hal_sm4_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
	
	unsigned char cbc,encrypt;
	unsigned char tmp_iv[16];
	unsigned int i,j;

	if(len==0 || len%16!=0)
	{
		return 1;	
	}
	
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		if (piv == NULL)
			return 2;
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}

	if(cbc == 0)
	{
		sms4_key_expend((unsigned long *)pkey);
		sms4_run((unsigned long *)pbin, (unsigned long *)pbout, len, encrypt);
	}
	else
	{
		sms4_key_expend((unsigned long *)pkey);
		memcpy(tmp_iv, piv, 16);
		
		for(i=0;i<len;i+=16)
		{
			for(j=0;j<16;j++) 
			{
				pbin[j] = pbin[i+j] ^ tmp_iv[j];
			}
			sms4_run((unsigned long *)pbin, (unsigned long *)tmp_iv, 16, encrypt);
			memcpy(pbout+i,tmp_iv,16);
		} 
	}

	return 0;
}
unsigned char hal_alg_bcaeInit(unsigned char *pkey,unsigned char *piv,unsigned char algtype,unsigned char mode)
{
	if(algtype==ALG_SM4)
	{
		
	}
	else if(algtype==ALG_SSF33)
	{
		
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_init(pkey,piv,mode);
	}
	else 
		return 1;

	return 0;
	
}
unsigned char hal_alg_bcaeCryp(unsigned char *pin,unsigned short len,unsigned char *pout,unsigned char algtype,unsigned char sm1_cbc_decryp )
{
	if(algtype==ALG_SM4)
	{
		
	}
	else if(algtype==ALG_SSF33)
	{
		
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_cryp(pin,len,pout,sm1_cbc_decryp);
	}
	else 
		return 1;

	return 0;
	
}
unsigned char hal_alg_bcae(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char mode,
								unsigned char *pbout,
								unsigned char algtype)
{
	if(algtype==ALG_SM4)
	{
		hal_sm4_oper(pbin,len,pkey,piv,mode,pbout);		
	}
	else if(algtype==ALG_SSF33)
	{
		hal_ssf33_oper(pbin,len,pkey,piv,mode,pbout);	
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_oper(pbin,len,pkey,piv,mode,pbout);	
	}
	else 
		return 1;

	return 0;
	
}
unsigned char hal_alg_calmac(unsigned char *pbin,unsigned short len,unsigned char *pkey,unsigned char *piv,unsigned char *pmac,unsigned char algtype)
{
	unsigned short tmplen=len;
	
	hal_alg_filldata(pbin,pbin,16,&tmplen);

	if(algtype==ALG_SM4)
	{
		hal_sm4_oper(pbin,tmplen,pkey,piv,SYMALG_CBC|SYMALG_SENC,pbin);		
	}
	else if(algtype==ALG_SSF33)
	{
		hal_ssf33_oper(pbin,tmplen,pkey,piv,SYMALG_CBC|SYMALG_SENC,pbin);	
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_oper(pbin,tmplen,pkey,piv,SYMALG_CBC|SYMALG_SENC,pbin);	
	}
	else 
		return 1;
	memcpy(pmac,pbin+tmplen-16,4);
	return 0;
	
}
//----------------------hash运算------------------
void hal_sha1_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pbout)
{
	HASH_CTX sha1_ctx;
	Hash_Initialize(&sha1_ctx, 1);
	Hash_Update(&sha1_ctx,pbin, len);	
	Hash_Finalize(&sha1_ctx);
	memcpy(pbout, sha1_ctx.ABCDEFGH, 20);
}
HASH_CTX pHash_Ctx;
void hal_alg_hashinit(unsigned type)
{
	Hash_Initialize(&pHash_Ctx, type);
}

void  hal_alg_hashupdate(unsigned char *pIn,unsigned int len)
{
	Hash_Update(&pHash_Ctx,pIn,len);
}
void  hal_alg_hashfinal(unsigned char *pOut)
{
	Hash_Finalize(&pHash_Ctx);
	memcpy(pOut,(unsigned char*)pHash_Ctx.ABCDEFGH,32);
}
//----------------------hash运算END------------------

unsigned char G_ALG_BUF[2048];

RSA_CRT_Key g_RsaKey;
RSA_PUBLICKEY g_rsapubkey;
RSA_CRT_PRIVATEKEY g_rsaprikey;
U32 *G_RSA_DATA_BUF;

void hal_RSAinit()
{
	U32 *p_buff=(U32*)G_ALG_BUF;

	
	g_RsaKey.pkp= p_buff;
	g_RsaKey.pkq =  p_buff+32*1;
	g_RsaKey.pkdp=  p_buff+32*2;
	g_RsaKey.pkdq=  p_buff+32*3;
	g_RsaKey.pku=  p_buff+32*4;

	g_RsaKey.pke = p_buff+32*5;
	g_RsaKey.pkn= p_buff+32*7;

	g_rsapubkey.pE= p_buff+32*5;
	g_rsapubkey.pN= p_buff+32*7;
	g_rsapubkey.pE[0] = 0x10001;

	g_rsaprikey.pP= p_buff;
	g_rsaprikey.pQ =  p_buff+32*1;
	g_rsaprikey.pDP=  p_buff+32*2;
	g_rsaprikey.pDQ=  p_buff+32*3;
	g_rsaprikey.pU=  p_buff+32*4;

	G_RSA_DATA_BUF=(U32 *)(G_ALG_BUF+1536);
	
	return;
}

void Char2IntReverse(unsigned char* in, unsigned int* out,unsigned int len)
{

	unsigned int temp[64],templen;
		
	templen=len/4;
	
	memcpy((unsigned char*)temp,in,len);
	for(; templen != 0;)
	{
	  *(out++) = temp[--templen];
	}

//	unsigned int len1;
//	unsigned char buf[4];
//	
//	len1=len/4;


	
}

void Int2CharReverse(unsigned int* in, unsigned char* out,unsigned int len)
{
	unsigned int temp[64],templen;
	templen=len;
	for(;len != 0;)
	{
		temp[--len] = *(in++);
	}
	memcpy(out,(unsigned char*)temp,templen*4);

}

void endiantrans(unsigned char *pbin,unsigned short len)
{
	unsigned char tmp;
	unsigned short i;

	for(i=0;i<len/2;i++)
	{
		tmp=pbin[i];
		pbin[i]=pbin[len-i-1];
		pbin[len-i-1]=tmp;
		
	}
}

//对待计算数据使用PKCS1_V1_5方式填充
unsigned char hal_alg_Rsapadding(		
							unsigned char* 		pInBuf,
							unsigned int		InLen,
							unsigned char* 		puchOutData,
							unsigned int		ulOutDataLen,
							unsigned char		type
							)
{
	unsigned int i, j;
	unsigned char *ptr;	
	
	if((type & ASYALG_ENDECIS) == ASYALG_ASIGN)//私钥填充
	{
			//PKCS1_V1_5方式填充私钥计算数据
		if (InLen > (ulOutDataLen - 11))
		{
			return NOK;
		}	
		ptr = puchOutData;
	
		*(ptr++) = 0;
		*(ptr++) = 1; /* Private Key BT (Block Type) */
	
		/* pad out with 0xff data */
		j = ulOutDataLen - 3 - InLen;
		memset(ptr, 0xff, j);
		ptr += j;
		*(ptr++) = '\0';
		memcpy(ptr, pInBuf, InLen);
	}
	else		//PKCS1_V1_5方式填充公钥计算数据
	{
		if (InLen > (ulOutDataLen - 11))
		{
			return NOK;
		}
	
		ptr = puchOutData;
	
		*(ptr++) = 0;
		*(ptr++) = 2; /* Public Key BT (Block Type) */
	
		/* pad out with non-zero random data */
		j = ulOutDataLen - 3 - InLen;	
		
		
		hal_alg_genrand(ptr,j);	//产生随机数	
		
		
		
		//检查随机数中是否有0，若有则重新生成随机数	
		for(i=0;i<j;i++)
		{
			if(*ptr == '\0')
			{
				do
				{
					hal_alg_genrand(ptr,1);
				}while(*ptr == '\0');
			}
			ptr++;			
		}	

		*(ptr++) = '\0';	
		memcpy(ptr, pInBuf, InLen);
	}

	return OK;
}
							
//从解密后数据中恢复原始数据
//ulFlags definition: bit0:0-rsa1024,1-rsa2048; bit1:reserved;bit2:0-PKCS1_V1_5方式填充,1-其它方式
unsigned char hal_alg_RsaOffPadding(		
							unsigned char* 		pInBuf,
							unsigned int		InLen,
							unsigned char* 		puchOutData,
							unsigned int*		pulOutDataLen)
{
	unsigned int i,j;
	unsigned char* ptr;
	ptr = pInBuf;

	if(*ptr)		//第一个字节必须是0
		return NOK;
	ptr++;	
	if(*ptr == 1)		//BT=1
	{
		ptr++;	//指向PS数据段
		j = InLen-2;
		for(i=0;i<j;i++)
		{
			if(*ptr != 0xff)	//检查PS段结束标志
			{
				if(*ptr == 0)
				{
					ptr++;
					break;
				}
				else
					return NOK;
			}
			ptr++;								
		}
		if(i == j)	//如果全是0xff，则错误
			return NOK;
		if(i < 8)	//如果PS段长度小于8字节，则错误
			return NOK;
		i++;
		*pulOutDataLen = j-i;	//有效数据长度
		memcpy(puchOutData, ptr, *pulOutDataLen);
	}
	else if(*ptr == 2)	//BT=2
	{
		j = InLen-2;
		for(i=0;i<j;i++)	//查找PS段结束标志'\0'
		{
			if(*(++ptr) == 0)
				break;				
		}
		if(i == j)	//如果全部是PS字段，则错误
			return NOK;
		if(i < 8)	//如果PS字段长度小于8，则错误
			return NOK;
		i++;
		*pulOutDataLen = j-i;
		ptr++;
		memcpy(puchOutData, ptr, *pulOutDataLen);
	}
	else
		return NOK;
	return OK;
}	

void hal_genRSAkey(RSA_CRT_Key *pRSAkey,unsigned char flag)
{
	unsigned short len,len1;

	if(flag==ASYALG_RSA1024)
	{
		len=1024;
		len1=128;
	}
	else if(flag==ASYALG_RSA2048)
	{
		len=2048;
		len1=256;
	}

	memset((U8*)pRSAkey->pke,0,256);
	pRSAkey->pke[0]=0X010001;

	RSA_Get_Key_For_CRT(pRSAkey->pku,
						pRSAkey->pke,
						pRSAkey->pkdp,
						pRSAkey->pkdq,
						pRSAkey->pkp,
						pRSAkey->pkq,
						pRSAkey->pkn,
						len);	

	Char2IntReverse((U8 *)pRSAkey->pkn,pRSAkey->pkn,len1);	
	Char2IntReverse((U8*)pRSAkey->pkp,pRSAkey->pkp,len1/2);
	Char2IntReverse((U8*)pRSAkey->pkq,pRSAkey->pkq,len1/2);
	Char2IntReverse((U8*)pRSAkey->pkdp,pRSAkey->pkdp,len1/2);
	Char2IntReverse((U8*)pRSAkey->pkdq,pRSAkey->pkdq,len1/2);
	Char2IntReverse((U8*)pRSAkey->pku,pRSAkey->pku,len1/2);
	return;
}

void hal_alg_RSApublic(RSA_PUBLICKEY *pubkey,unsigned char *pIn,unsigned char *pOut,unsigned char flag)
{
	unsigned int len,len2;
	
//	memset((U8*)pubkey->pE,0,256);
	pubkey->pE[0]=0x10001;

	if(flag==ASYALG_RSA1024)
	{
		len=1024;
		len2=128;
	}
	else if(flag==ASYALG_RSA2048)
	{
		len=2048;
		len2=256;
	}
	//memcpy(G_RSA_DATA_BUF,pIn,len);
	Char2IntReverse(pIn,G_RSA_DATA_BUF,len2);	
	Char2IntReverse((U8*)pubkey->pN,pubkey->pN,len2);
	AEmodN_for_RSA (G_RSA_DATA_BUF, G_RSA_DATA_BUF, pubkey->pE, pubkey->pN, len);
	Int2CharReverse(G_RSA_DATA_BUF, pOut,len2/4);
	//memcpy(pOut,G_RSA_DATA_BUF,len);


}

void hal_alg_RSAprivate(RSA_CRT_PRIVATEKEY *privatekey,unsigned char *pIn,unsigned char *pOut,unsigned char flag)
{
	unsigned int len,len1;
	
	if(flag==ASYALG_RSA1024)
	{
		len=128;
		len1=1024;
	}
	else if(flag==ASYALG_RSA2048)
	{
		len=256;
		len1=2048;
	}
	Char2IntReverse(pIn,G_RSA_DATA_BUF,len);
	#if 1
	Char2IntReverse((U8*)privatekey->pP,privatekey->pP,len/2);
	Char2IntReverse((U8*)privatekey->pQ,privatekey->pQ,len/2);
	Char2IntReverse((U8*)privatekey->pDP,privatekey->pDP,len/2);
	Char2IntReverse((U8*)privatekey->pDQ,privatekey->pDQ,len/2);
	Char2IntReverse((U8*)privatekey->pU,privatekey->pU,len/2);	
	#endif

#if 0
	UART_printf("rsa private key P\r\n");
	HAL_UART_SEND_ASC((U8*)privatekey->pP,64);
	UART_printf("rsa private key Q\r\n");
	HAL_UART_SEND_ASC((U8*)privatekey->pQ,64);
	UART_printf("rsa private key dP\r\n");
	HAL_UART_SEND_ASC((U8*)privatekey->pDP,64);
	UART_printf("rsa private key dQ\r\n");
	HAL_UART_SEND_ASC((U8*)privatekey->pDQ,64);
	UART_printf("rsa private key U\r\n");
	HAL_UART_SEND_ASC((U8*)privatekey->pU,64);
#endif	
	RSA_bigint_mod_e_CRT(G_RSA_DATA_BUF,
						privatekey->pU,
						privatekey->pDP,
						privatekey->pDQ,
						privatekey->pP,
						privatekey->pQ,
						G_RSA_DATA_BUF,
						len1);

	
	Int2CharReverse(G_RSA_DATA_BUF, pOut,len/4);

}

static ECC_CTX pEcc_ctx;
static U32 ECCData[256];
U32 *G_SM2_IN_BUF;
U32 *G_SM2_OUT_BUF;
SM2_KEYEXCHANGE *p_sm2_keyexchange;
SM2_KEYEXCHANGE1  *p_sm2_keyA;

U32 SM2_Para[] =
{256,	
0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0xFFFFFFFC, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x4D940E93, 0xDDBCBD41, 0x15AB8F92, 0xF39789F5, 0xCF6509A7, 0x4D5A9E4B, 0x9D9F5E34, 0x28E9FA9E,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x334C74C7, 0x715A4589, 0xF2660BE1, 0x8FE30BBF, 0x6A39C994, 0x5F990446, 0x1F198119, 0x32C4AE2C,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x2139F0A0, 0x02DF32E5, 0xC62A4740, 0xD0A9877C, 0x6B692153, 0x59BDCEE3, 0xF4F6779C, 0xBC3736A2,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
void hal_sm2_init(void)
{

	ECC_LoadPara(&pEcc_ctx, (U8*)SM2_Para, ECCData);
	G_SM2_IN_BUF=(U32 *)G_ALG_BUF;
	G_SM2_OUT_BUF=(U32 *)(G_ALG_BUF+1024);
	p_sm2_keyexchange=(SM2_KEYEXCHANGE*)G_ALG_BUF;

}

unsigned char hal_alg_SM2producekeys(U8  *pPubkey, U8 *pPrikey)
{
	
	unsigned int ret;
	U32 Pubkey[16],Prikey[8];
	
	ret = ECC_ProduceKeys(&pEcc_ctx, Pubkey, Prikey);//生成密钥对
	if(ret)
	{
		return 1;
	}

	Int2CharReverse(Pubkey, pPubkey, 8);
	Int2CharReverse(Pubkey+8, pPubkey+32, 8);
	Int2CharReverse(Prikey, pPrikey, 8);
		
	return 0;	
}
//flag 0: C1 +C2+C3 flag:1 C1+C3+C2
unsigned char hal_alg_SM2encrypt(U8 *Pubkey,U8 *pIn,unsigned int inlen,U8 *pOut,U8 flag)
{
	unsigned char ret;
	U8 mac[32];	
	U32 pub_key[16];	

	memcpy((U8*)G_SM2_IN_BUF,pIn,inlen);

	Char2IntReverse(Pubkey, pub_key, 32);
	Char2IntReverse(Pubkey+32, pub_key+8, 32);
	
	ret = SM2_Encrypt(&pEcc_ctx, pub_key, G_SM2_IN_BUF, inlen, G_SM2_OUT_BUF); //
	if(ret==0)
	{
		Int2CharReverse(G_SM2_OUT_BUF, pOut, 8);
		Int2CharReverse(G_SM2_OUT_BUF+8, pOut+32, 8);
		if(flag==0)
		{
			memcpy(mac,(unsigned char *)(G_SM2_OUT_BUF+16),32);
			memcpy(pOut+64,(unsigned char *)(G_SM2_OUT_BUF+24),inlen);
			memcpy(pOut+64+inlen,mac,32);
		}
		
	}

	return ret;
		
}

unsigned char hal_alg_SM2decrypt(U8 *Prikey,unsigned char *pIn,unsigned int inlen,unsigned char *pOut)
{
	unsigned char ret;
	U8 mac[32];
	U32 pri_key[8];
	
	memcpy(mac,pIn+inlen-32,32);
	memmove(pIn+96,pIn+64,inlen-96);
	memcpy(pIn+64,mac,32);

	memcpy((U8*)G_SM2_IN_BUF,pIn,inlen);
	
	Char2IntReverse(Prikey, pri_key, 32);
	Char2IntReverse((U8*)G_SM2_IN_BUF, G_SM2_IN_BUF, 32);//X
	Char2IntReverse((U8*)(G_SM2_IN_BUF+8), G_SM2_IN_BUF+8, 32);//Y	
	
	ret = SM2_Decrypt(&pEcc_ctx, pri_key, G_SM2_IN_BUF, inlen,G_SM2_OUT_BUF);

	memcpy(pOut,G_SM2_OUT_BUF,inlen-96);

	return ret;
}


unsigned char hal_alg_SM2sign(U8 *Prikey, U8 *pHash, U8 *pSign)
{
	unsigned char ret;

	U32 pri_key[8],hash[8],sign[16];


	Char2IntReverse(Prikey, pri_key, 32);
	Char2IntReverse(pHash, hash, 32);	
	
	ret = SM2_Sign(&pEcc_ctx, pri_key, hash, sign); //
	if(ret==0)
	{
		Int2CharReverse(sign, pSign, 8);
		Int2CharReverse(sign+8, pSign+32, 8);
	}
	return ret;
}

unsigned char hal_alg_SM2verify(U8 *Pubkey,  U8*pHash,U8 *pSign)
{
	unsigned char ret;
	U32 pub_key[16],hash[8],sign[16];

	Char2IntReverse(Pubkey, pub_key, 32);
	Char2IntReverse(Pubkey+32, pub_key+8, 32);
	Char2IntReverse(pHash, hash, 32);
	Char2IntReverse(pSign, sign, 32);
	Char2IntReverse(pSign+32, sign+8, 32);
	
	ret = SM2_Verify(&pEcc_ctx, pub_key, hash, sign); //
	return ret;
}

void hal_alg_SM2getz(unsigned char *pubkey,unsigned char *pid,unsigned char idlen,unsigned char *pZ)
{
	U32 pub_key[16];

	Char2IntReverse(pubkey, pub_key, 32);
	Char2IntReverse(pubkey+32, pub_key+8, 32);
	GetZ(&pEcc_ctx,(U32*)pid, idlen, pub_key,(U32*)pZ);
}

unsigned char hal_alg_SM2genexchangekey(SM2_KEYEXCHANGE *p_sm2_keyex,unsigned char *pout,unsigned char mode)
{
	U8 key[32],ret;
	p_sm2_keyA=(SM2_KEYEXCHANGE1*)(G_ALG_BUF+1024);
	
	Char2IntReverse(p_sm2_keyex->selfpub, p_sm2_keyA->selfpub, 32);
	Char2IntReverse(p_sm2_keyex->selfpub+32,  p_sm2_keyA->selfpub+8, 32);
	Char2IntReverse(p_sm2_keyex->selfpri, p_sm2_keyA->selfpri, 32);

	Char2IntReverse(p_sm2_keyex->selftmppub, p_sm2_keyA->selftmppub, 32);
	Char2IntReverse(p_sm2_keyex->selftmppri, p_sm2_keyA->selftmppri, 32);

	Char2IntReverse(p_sm2_keyex->otherpub, p_sm2_keyA->otherpub, 32);
	Char2IntReverse(p_sm2_keyex->otherpub+32,  p_sm2_keyA->otherpub+8, 32);

	Char2IntReverse(p_sm2_keyex->othertmppub, p_sm2_keyA->othertmppub, 32);
	Char2IntReverse(p_sm2_keyex->othertmppub+32, p_sm2_keyA->othertmppub+8, 32);

	p_sm2_keyA->selfidlen=p_sm2_keyex->selfidlen;
	memcpy((U8*)p_sm2_keyA->selfid,p_sm2_keyex->selfid,p_sm2_keyA->selfidlen);

	p_sm2_keyA->otheridlen=p_sm2_keyex->otheridlen;
	memcpy((U8*)p_sm2_keyA->otherid,p_sm2_keyex->otherid,p_sm2_keyA->otheridlen);

	ret=SM2_KeyExchange(&pEcc_ctx,
					p_sm2_keyA->selfpub,
					p_sm2_keyA->selfpri,
					p_sm2_keyA->selftmppub,
					p_sm2_keyA->selftmppri,
					p_sm2_keyA->otherpub,
					p_sm2_keyA->othertmppub,
					p_sm2_keyA->selfid,
					p_sm2_keyA->selfidlen,
					p_sm2_keyA->otherid,
					p_sm2_keyA->otheridlen,	
					(U32*)key,
					16,mode	);

	memcpy(pout,key,16);
	return ret;
}

void hal_alg_init(void)
{
	REAECSR = 0x00000008;
	hal_RSAinit();
	SM1_Init();
	hal_sm2_init();
}

#if 0
void RSA_test()
{
	
	unsigned char data[128]={0,1,2,3,4,5,6,7,8};
	unsigned char data1[128];

	unsigned char P[]={0x0B,0xBB,0x6B,0x47,0xD4,0x5E,0xD8,0x72,0x77,0xDD,0xC5,0x39,0x5C,0x08,0x71,0xBD,0x07,0xF3,0xF4,0x84,0xEA,0x51,0x31,0xA6,0x7E,0x60,0x48,0xE3,0xDC,0x3C,0x5A,0xE7,0xAD,0x15,0x9A,0x65,0x27,0x05,0xB5,0x48,0x38,0xE7,0x72,0xE2,0x0B,0xCC,0x57,0xD8,0x10,0x87,0x10,0x4F,0x81,0xAE,0x7C,0x19,0x17,0xDF,0x7E,0x7C,0x8A,0x17,0xD4,0xFD};
	unsigned char Q[]={0xCF,0x8E,0xA6,0x6D,0xBA,0x88,0xE6,0xCE,0xF5,0x20,0x96,0x76,0x39,0x6F,0xC2,0x48,0x74,0x9A,0x0C,0x0C,0x76,0x4D,0xCC,0x68,0x02,0x36,0x26,0x6E,0xD8,0xBE,0xBD,0xCF,0x6A,0xB8,0xC1,0xFE,0xFE,0xEB,0x06,0xD2,0x08,0x2F,0xB6,0xCF,0x8F,0x31,0x87,0xC1,0x4E,0xC5,0x39,0x54,0x8E,0x75,0x18,0x5E,0x4D,0x08,0xE6,0xE1,0x03,0x8B,0x2F,0xEB};
	unsigned char DP[]={0x77,0x36,0xDA,0x17,0xFB,0xE4,0x29,0x58,0x01,0x8C,0x0F,0xA1,0x78,0xD8,0x98,0x84,0x7A,0x30,0xEE,0xBF,0x59,0x33,0xD8,0xBA,0xC7,0xFF,0xDF,0x91,0xA9,0xF3,0x22,0xC5,0xA6,0xE7,0x96,0x2C,0x5A,0xFC,0x9F,0xCA,0x8C,0x99,0x24,0x6F,0xFC,0x88,0x17,0x9B,0x03,0xB5,0xD1,0xA4,0x70,0xCB,0x6C,0xD7,0x3E,0xB0,0xC5,0x0E,0x11,0xF2,0x81,0x90};
	unsigned char DQ[]={0x05,0xC3,0x98,0x8E,0xBB,0xEE,0xD6,0x99,0xCE,0x0F,0xF2,0xFE,0xF1,0x01,0x7B,0x55,0xD3,0x05,0xFF,0x68,0x40,0xD9,0xD5,0x5F,0x66,0x56,0x0A,0x6D,0xE3,0x53,0xF9,0x4E,0x0C,0x79,0xD7,0x45,0x75,0xB6,0x6C,0xEB,0xF1,0x37,0x73,0x40,0x35,0xC3,0xCC,0x84,0x3D,0xDA,0x9B,0x6D,0x2B,0x2F,0x5C,0xF2,0xDE,0xE1,0xEE,0xDA,0x2E,0xBF,0x76,0x62};
	unsigned char U[]={0x74,0x0C,0x67,0x71,0xB4,0xEF,0x99,0xB0,0x09,0x2A,0x4C,0xF2,0x13,0x9A,0x81,0xBD,0xB2,0x77,0x63,0x6B,0xB6,0x33,0x6B,0xCD,0xDE,0x44,0x1E,0x64,0x8B,0xB5,0xB5,0xD9,0x10,0x84,0x15,0xF7,0xA6,0xC8,0x58,0xEE,0x0C,0x76,0x8E,0xD6,0x97,0x17,0x48,0x83,0x67,0x3D,0xAE,0xB0,0x6D,0xB8,0xB4,0x8C,0xF5,0x4E,0x89,0x66,0xB2,0x51,0xF4,0xC7};
	unsigned char N[]={0xE5,0x57,0xFE,0x79,0xC0,0x59,0x23,0xC7,0xF8,0xD6,0xE4,0xD2,0x97,0x7E,0x3C,0x92,0x7F,0xD3,0x88,0xEE,0xCF,0xD6,0x55,0x60,0x26,0x66,0xCA,0xC2,0xD3,0xBC,0xF7,0x09,0xED,0xF9,0xFD,0x5A,0xFF,0x7D,0xC1,0x1D,0x5D,0xBB,0x69,0x4B,0x84,0xE0,0xD9,0xB4,0xCD,0x0D,0xF5,0xA7,0xAA,0xBA,0xC6,0xCE,0x50,0xF3,0x2E,0x23,0x69,0xDA,0xAA,0xB3,0x94,0xDA,0xF9,0x32,0x12,0xBF,0x4A,0x11,0xB8,0x52,0x12,0x29,0x45,0x8C,0xE8,0x84,0xE9,0xEB,0x6A,0x3B,0xD4,0x34,0xF8,0x1B,0xD7,0xFA,0x37,0x35,0xC6,0xFA,0xEA,0xC1,0xE2,0xEC,0x6A,0xF2,0xF5,0x64,0xAA,0x48,0xE6,0x62,0x11,0xD0,0x6C,0xA4,0x01,0x93,0xB5,0xBA,0x6D,0xFE,0xE1,0x2F,0xEC,0xB6,0xC6,0x47,0xF5,0x74,0x62,0xD5,0x30,0xE9};
	

	//UART_printf("GEN RSA KEY \r\n");
	//hal_genRSAkey(&g_RsaKey,ASYALG_RSA1024);	

	memcpy(g_rsaprikey.pP,P,64);
	memcpy(g_rsaprikey.pQ,Q,64);
	memcpy(g_rsaprikey.pDP,DP,64);
	memcpy(g_rsaprikey.pDQ,DQ,64);
	memcpy(g_rsaprikey.pU,U,64);

	memcpy(g_rsapubkey.pN,N,128);	

	UART_printf("加密\r\n");
	hal_alg_RSAprivate(&g_rsaprikey,data,data1,ASYALG_RSA1024);
	UART_printf("解密\r\n");
	hal_alg_RSApublic(&g_rsapubkey,data1,data,ASYALG_RSA1024);	

	HAL_UART_SEND_ASC(data,128);
	
	
}
#endif

