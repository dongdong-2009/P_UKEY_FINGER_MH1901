#include "app_cmd.h"
#include "cos.h"
#include "hal_alg.h"
#include "base_defines.h"

unsigned char g_rand[8],g_randstate=0;
extern unsigned char g_reboot;
SESSION_KEY g_session_key;
unsigned char g_same_cmd=0;
const unsigned char app_ver[]="20170605";
APDU g_APDU;

# if 0


//格式化设备信息
unsigned short APP_CMD_Format(void)
{
	return SW_SUCCESS;	 
}

unsigned short APP_CMD_GenRand(void)
{
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	
	hal_alg_genrand(g_APDU.pOutData,g_APDU.LC);
	memcpy(g_rand,g_APDU.pOutData,8);
	g_randstate=1;
	
	g_APDU.RLE=g_APDU.LC;
	
	return SW_SUCCESS;	 
}
unsigned short APP_CMD_ReadTerminalInfo(void)
{
	unsigned short len=0;
	unsigned char *pout;
	MANUFACTURER_INFO *p_info;
	DEV_AUTHKEY_INFO  *p_authkey;
	unsigned int total_size,rest_size;
	unsigned char chipNo[8];
	g_APDU.RLE=0;

	pout=g_APDU.pOutData;
	p_info=(MANUFACTURER_INFO*)DEV_VER_INFO;
	p_authkey=(DEV_AUTHKEY_INFO*)DEV_AUTH_KEY;
	
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0)
	{
		if(g_APDU.LC!=0x7b)
			return SW_LCLE_ERROR;
		
		memcpy(pout,p_info->hardware_ver,96);//厂商信息+序列号
		pout+=96-16;//设备序列号由芯片序列号转换

		//memcpy(g_APDU.pOutData+2,"01",2);//指纹算法

		cos_get_devicenum(pout);
		pout+=16;

		memcpy(pout,(unsigned char*)0x800,16);//芯片序列号
		pout+=16;

		cos_getsizeinfo(&total_size,&rest_size);
		memcpy(pout,(unsigned char*)&total_size,4);//COS总空间
		pout+=4;
		
		memcpy(pout,(unsigned char*)&rest_size,4);//COS总空间
		pout+=4;

		*pout++=p_authkey->retry_counter;
		*pout++=0X04;
		*pout++=0X00;
		
		g_APDU.RLE=0x7b;
	}
	else if(g_APDU.PP2==1)//获取介质标号
	{
		if(g_APDU.LC!=0x20)
			return SW_LCLE_ERROR;
		return cos_get_keynum(0,g_APDU.pOutData,&g_APDU.RLE);
		
	}
	else if(g_APDU.PP2==2)
	{
		if(g_APDU.LC!=0x80)
			return SW_LCLE_ERROR;
		return cos_get_keynum(1,g_APDU.pOutData,&g_APDU.RLE);
	}
	else if(g_APDU.PP2==3)
	{
		if(g_APDU.LC!=0x02)
			return SW_LCLE_ERROR;
		cos_version(g_APDU.pOutData);
		g_APDU.RLE=2;
	}
	else if(g_APDU.PP2==4)
	{
		if(g_APDU.LC!=0x01)
			return SW_LCLE_ERROR;
		g_APDU.pOutData[0]=cos_getsecurestate();
		g_APDU.RLE=1;
	}
	else if(g_APDU.PP2==0xAA)//获取介质标号
	{		
		U8 flag[4]={0x12,0x34,0x56,0x78};
	
		if(memcmp(p_info->init_flag,flag,4)!=0)
			return SW_INFO_NOINIT;
		memcpy(pout,p_info->hardware_ver,96);//厂商信息+序列号
		pout+=96-16;//设备序列号由芯片序列号转换

		memcpy(g_APDU.pOutData+2,"01",2);//指纹算法

		memcpy(chipNo,(unsigned char*)0x800,8);//设备序列号
		TOOL_BcdToAsc(pout,chipNo,8);
		pout+=16;		
		g_APDU.RLE=96;
	}
	else 
		return SW_P1P2_ERROR;
	
	return SW_SUCCESS;	 
}


//设置介质标号
unsigned short APP_CMD_SetKeyNum(void)
{
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0)
	{
		if(g_APDU.LC!=0x20)
			return SW_LCLE_ERROR;
		return cos_set_keynum(0,g_APDU.pInData);
	}

	else if(g_APDU.PP2==1)
	{
		if(g_APDU.LC!=0x80)
			return SW_LCLE_ERROR;
		return cos_set_keynum(1,g_APDU.pInData);
	}
}

//修改认证密钥
unsigned short APP_CMD_ChangeDevPin(void)
{	
	unsigned char rand[16]={0};
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x24)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;

	memcpy(rand,g_rand,8);

	memmove(g_APDU.pInData+6,g_APDU.pInData,g_APDU.LC);
	memcpy(g_APDU.pInData,(unsigned char *)&g_APDU.CLA,6);
	return cos_changedevkey(g_APDU.pInData,rand,g_APDU.PP2);
}

//获取PIN信息
unsigned short APP_CMD_GetPinInfo(void)
{	
	unsigned short ret;
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)
	{
		 g_APDU.RLE=2;
		return cos_getpininfo(g_APDU.pOutData,g_APDU.PP2);
	}

	return ret;
}

unsigned short APP_CMD_BlockPin(void)
{	
	unsigned short ret;
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)
	{
		return cos_blockuserpin();
	}
	return ret;
}
unsigned short APP_CMD_ClearSecureState(void)
{	
	unsigned short ret;
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)
	{
		return cos_clearsecurestate();
	}
	return ret;
}


unsigned short APP_CMD_ChangePin(void)
{	
	unsigned short ret;
	unsigned char rand[16]={0};
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x26)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;

	memcpy(rand,g_rand,8);

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)
		return ret;

	memmove(g_APDU.pInData+6,g_APDU.pInData,g_APDU.LC);
	memcpy(g_APDU.pInData,(unsigned char *)&g_APDU.CLA,6);
	
	return cos_changepin(g_APDU.pInData,rand,g_APDU.PP1,g_APDU.PP2);
}

unsigned short APP_CMD_UnlockPin(void)
{	
	unsigned short ret;
	unsigned char rand[16]={0};
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x26)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;

	memcpy(rand,g_rand,8);

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)
		return ret;
	memmove(g_APDU.pInData+6,g_APDU.pInData,g_APDU.LC);
	memcpy(g_APDU.pInData,(unsigned char *)&g_APDU.CLA,6);
	return cos_unblock_userpin(g_APDU.pInData,rand,USER_PIN,g_APDU.PP2,g_APDU.PP1);
}
//创建应用的同时创建PIN文件
unsigned short APP_CMD_CreatAPP(void)
{
	unsigned short file_id,ret,file_size;
	CREATE_APP *p_create_app=(CREATE_APP*)g_APDU.pInData;
	unsigned char auth[3];

	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0x47)
		return SW_LCLE_ERROR;

	file_id=p_create_app->fid[0]<<8+p_create_app->fid[1];

	//配置应用权限
	auth[0]=AUTH_FREE;//读权限自由
	auth[1]=p_create_app->auth;//应用下创建文件权限
	auth[2]=AUTH_DEV;//删除应用需要设备权限
#if 0	
	if(FID_MF==file_id)
	{
		ret=cos_check_MF();
		if(SW_MF_NOTEXIST==ret)
		{
			ret=cos_creat_app(p_create_app->fid,0x0f,p_create_app->auth,p_create_app->name,p_create_app->admin_pin);
			UART_printf("file data\r\n");
			HAL_UART_SEND_ASC((unsigned char *)MFSTART,512);			
		}
		return ret;
	}
	else
#endif
	{
		ret=cos_check_MF();
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_auth(F_TPYE_MF,AUTH_WRITE_LOC);
			if(SW_SUCCESS==ret)
			{
				ret=cos_check_FIDok(p_create_app->fid,F_TPYE_DF);
				if(SW_SUCCESS==ret)
				{
					ret=cos_creat_app(p_create_app->fid,auth,p_create_app->name,p_create_app->admin_pin);
				}			
			}
		}

		return ret;
	}
}        

unsigned short APP_CMD_OpenAPP(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0&&g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;
	else if(g_APDU.PP2==1&&g_APDU.LC!=0x20)
		return SW_LCLE_ERROR;
	return cos_open_app(g_APDU.pInData,g_APDU.PP2);
}

unsigned short APP_CMD_CloseAPP(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0&&g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;
	else if(g_APDU.PP2==1&&g_APDU.LC!=0x20)
		return SW_LCLE_ERROR;
	return cos_close_app(g_APDU.pInData,g_APDU.PP2);
}

unsigned short APP_CMD_DeleteAPP(void)
{
	unsigned short ret,file_size;
	unsigned char app_id[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;
	unsigned char secure_bak;

	g_APDU.RLE=0;
		
	if(g_APDU.LC!=0x2&&g_APDU.LC!=0X20)
		return SW_LCLE_ERROR;

	secure_bak=cos_getsecurestate();//打开应用会改变权限	
	ret=cos_open_app(g_APDU.pInData,g_APDU.PP2);
	cos_setsecurestate(secure_bak);
	if(ret==SW_SUCCESS)		
	{		

		if(secure_bak==SECURE_DEV)
		{
			ret=cos_delete_app();
		}
		else 
			return SW_SECURITY_DISSATISFY;
			
		
	}	
	return ret;
	
}

unsigned short APP_CMD_EnumAPP(void)
{
	unsigned short ret;
	
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0x00)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0)
		return SW_LCLE_ERROR;

	ret=cos_check_MF();
	if(ret==SW_SUCCESS)
	{
		return cos_enum_app(g_APDU.pOutData,&g_APDU.RLE);
	}
	
	return ret;
}

unsigned short APP_CMD_CreatFile(void)
{
	unsigned short ret,file_size;
	unsigned char app_id[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;
		
	if(g_APDU.LC!=0x28)
		return SW_LCLE_ERROR;

	app_id[0]=g_APDU.PP1;
	app_id[1]=g_APDU.PP2;
	ret=cos_open_app(app_id,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_check_auth(F_TPYE_DF,AUTH_WRITE_LOC);
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_FIDok(p_create_file->fid,F_TYPE_EF);
			if(SW_SUCCESS==ret){
				return cos_creat_file(p_create_file);
			}
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_DeleteFile(void)
{
	unsigned short ret,file_size;
	unsigned char app_id[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;
		
	if(g_APDU.LC!=0x2)
		return SW_LCLE_ERROR;

	app_id[0]=g_APDU.PP1;
	app_id[1]=g_APDU.PP2;
	ret=cos_open_app(app_id,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData,0);
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_auth(F_TYPE_EF,AUTH_DELETE_LOC);
			if(SW_SUCCESS==ret)
				ret=cos_delete_file();
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_WriteFile(void)
{
	unsigned short ret,file_len,offset;
	unsigned char app_id[2],fid[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
		
	if(g_APDU.LC<0x08)
		return SW_LCLE_ERROR;
	
	offset=(g_APDU.pInData[4]<<8)+g_APDU.pInData[5];	
	file_len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];	

	if(g_APDU.LC!=0x08+file_len)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData+2,0);
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
			if(SW_SUCCESS==ret)
				return cos_write_file(offset,g_APDU.pInData+8,file_len);
		}
	}	
	return ret;
	
}
unsigned short APP_CMD_GetFileInfo(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1)
		return SW_P1P2_ERROR;	
	
	ret=cos_open_app(g_APDU.pInData,g_APDU.PP2);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData+2,g_APDU.PP2);
		if(SW_SUCCESS==ret)
		{	
//			ret=cos_check_auth(F_TYPE_EF,AUTH_READ_LOC);
			if(SW_SUCCESS==ret)
			{
				ret=cos_get_fileinfo(g_APDU.pOutData,&g_APDU.RLE);
			}
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_ReadFile(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x08)
		return SW_LCLE_ERROR;

	offset=(g_APDU.pInData[4]<<8)+g_APDU.pInData[5];	
	file_len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];	
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData+2,g_APDU.PP2);
		if(SW_SUCCESS==ret)
		{	
			ret=cos_check_auth(F_TYPE_EF,AUTH_READ_LOC);
			if(SW_SUCCESS==ret)
			{
				ret=cos_read_file(g_APDU.pOutData,offset,file_len);
				g_APDU.RLE=file_len;
			}
		}
	}	
	return ret;
	
}
unsigned short APP_CMD_EnumFile(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_enum_file(g_APDU.pOutData,&g_APDU.RLE,g_APDU.PP1,g_APDU.PP2);
	}	
	return ret;
	
}
//-----------------------秘钥运算指令-----------------
unsigned short APP_CMD_Gen1024RSA(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_genrsakey(g_APDU.pInData+2,g_APDU.pInData+4,g_APDU.pOutData,ASYALG_RSA1024);	
		if(ret==SW_SUCCESS)
		{
			g_APDU.RLE=0x80;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_Gen2048RSA(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_genrsakey(g_APDU.pInData+2,g_APDU.pInData+4,g_APDU.pOutData,ASYALG_RSA2048);	
		if(ret==SW_SUCCESS)
		{
			g_APDU.RLE=0x100;
		}
	}	
	return ret;
	
}
unsigned short APP_CMD_1024RSAEncrypt_Verify(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;
		if(g_APDU.LC!=0x102)
			return SW_LCLE_ERROR;
		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
		flag=0;
		if(g_APDU.LC!=0x82)
			return SW_LCLE_ERROR;
	}
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		if(flag==1)//使用外部公钥
		{
			ret=cos_rsa1024_encrypt_verify(g_APDU.pInData+2+0x80,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_rsa1024_encrypt_verify(fid,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}	
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x80;
	}	
	return ret;
	
}
unsigned short APP_CMD_2048RSAEncrypt_Verify(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;
		if(g_APDU.LC!=0x202)
			return SW_LCLE_ERROR;
		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
		flag=0;
		if(g_APDU.LC!=0x102)
			return SW_LCLE_ERROR;
	}
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		if(flag==1)//使用外部公钥
		{
			ret=cos_rsa2048_encrypt_verify(g_APDU.pInData+2+0x100,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_rsa2048_encrypt_verify(fid,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}	
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x100;
	}	
	return ret;
	
}

unsigned short APP_CMD_1024RSADecrypt_Sign(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.LC!=0x82)
		return SW_LCLE_ERROR;
	
	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_rsa1024_decrypt_sign(fid,g_APDU.pInData+2,g_APDU.pOutData);
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x80;
	}	
	return ret;
	
}
unsigned short APP_CMD_2048RSADecrypt_Sign(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.LC!=0x102)
		return SW_LCLE_ERROR;
	
	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_rsa2048_decrypt_sign(fid,g_APDU.pInData+2,g_APDU.pOutData);
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x100;
	}	
	return ret;
	
}
unsigned short APP_CMD_RSAEmportKey(void)
{
	unsigned short ret,len;
	unsigned char rsatype;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x87&&g_APDU.LC!=0x107)
		return SW_LCLE_ERROR;

	len=(g_APDU.pInData[5]<<8)+g_APDU.pInData[6];//模长
	
	if(len==0x80)
	{
		rsatype=ASYALG_RSA1024;
	}
	else if(len==0x100)
	{
		rsatype=ASYALG_RSA2048;
	}
	else
		return SW_REFERENCEDATA_INVALID;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	g_session_key.state=0XAA;

	g_session_key.keyid[0]=g_APDU.pInData[2];
	g_session_key.keyid[1]=g_APDU.pInData[3];

	g_session_key.type=g_APDU.pInData[4];

	hal_alg_genrand(g_session_key.key,16);
	
	return cos_rsa_exportkey(g_APDU.pInData+7,g_session_key.key,rsatype,g_APDU.pOutData,&g_APDU.RLE);	
	
}

unsigned short APP_CMD_ExportRSAkeyA(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x04)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_exportpubkey(g_APDU.pInData+2,g_APDU.pOutData,&g_APDU.RLE);	
	
}
unsigned short APP_CMD_ImportRSAkeyA(void)
{
	unsigned short ret;
	unsigned char flag;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC==0x86)
	{
		flag=ASYALG_RSA1024;
	}
	else if(g_APDU.LC==0x106)
	{
		flag=ASYALG_RSA2048;
	}
	else
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_importpubkey(g_APDU.pInData+2,g_APDU.pInData+6,flag);	
	
}
unsigned short APP_CMD_ImportRSAkeyB(void)
{
	unsigned short ret;
	unsigned char flag;
	unsigned short len;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;

	len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];//模长
	
	if(len==0x400)
	{
		flag=ASYALG_RSA1024;
	}
	else if(len==0x800)
	{
		flag=ASYALG_RSA2048;
	}
	else
		return SW_REFERENCEDATA_INVALID;

	if(g_session_key.state!=0xaa)
		return SW_SECURITY_DISSATISFY;

	len=g_APDU.LC-8;
		
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_import_enc_prikey(g_APDU.pInData+2,g_session_key.key,
						g_session_key.type,g_APDU.pInData+8,len,flag);	
	
}
unsigned short APP_CMD_ImportRSAkeyB1(void)
{
	unsigned short ret;
	unsigned char flag;
	unsigned short len;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;

	len=(g_APDU.pInData[4]<<8)+g_APDU.pInData[5];//模长
	
	if(len==0x400)
	{
		flag=ASYALG_RSA1024;
	}
	else if(len==0x800)
	{
		flag=ASYALG_RSA2048;
	}
	else
		return SW_REFERENCEDATA_INVALID;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_importprikey(g_APDU.pInData+2,g_APDU.pInData+6,flag);	
	
}
unsigned short APP_CMD_GenSM2Key(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_genSM2key(g_APDU.pInData+2,g_APDU.pInData+4,g_APDU.pOutData,&g_APDU.RLE);				
	}	
	return ret;
	
}
unsigned short APP_CMD_SM2Encrypt(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;
		len=g_APDU.LC-66;	
	}
	else
	{
		len=g_APDU.LC-2;
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
	}
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		if(flag==1)
		{
			ret=cos_SM2encrypt(g_APDU.pInData+2+len,g_APDU.pInData+2,len,g_APDU.pOutData,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_SM2encrypt(fid,g_APDU.pInData+2,len,g_APDU.pOutData,flag);	
		}
		if(ret==SW_SUCCESS)
			g_APDU.RLE=len+96;
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2Decrypt(void)
{
	unsigned short ret;
	unsigned char fid[2];
	unsigned int len;

	g_APDU.RLE=0;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	

	len=g_APDU.LC-2;
		
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_SM2decrypt(fid,g_APDU.pInData+2,len,g_APDU.pOutData);	
		if(ret==SW_SUCCESS)
			g_APDU.RLE=len-96;
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2Sign(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;

	g_APDU.RLE=0;

	if(g_APDU.LC!=0x22)
		return SW_LCLE_ERROR;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	
		
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_SM2sign(fid,g_APDU.pInData+2,g_APDU.pOutData);
		if(ret==SW_SUCCESS)
			g_APDU.RLE=64;
	}	
	return ret;	
}

unsigned short APP_CMD_SM2Verify(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.LC!=0x62&&g_APDU.LC!=0xA2)
		return SW_LCLE_ERROR;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
	}
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		if(flag==1)
		{
			ret=cos_SM2verify(g_APDU.pInData+2,g_APDU.pInData+2+64,g_APDU.pInData+2+64+32,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_SM2verify(fid,g_APDU.pInData+2,g_APDU.pInData+2+32,flag);	
		}			
	}	
	return ret;
	
}

unsigned short APP_CMD_ExportSM2keyA(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x04)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_exportkeyA(g_APDU.pInData+2,g_APDU.pOutData,&g_APDU.RLE);	
	
}
unsigned short APP_CMD_ImportSM2KeyA(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x44)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_importpubkey(g_APDU.pInData+2,g_APDU.pInData+4);	
	
}

unsigned short APP_CMD_ImportSM2KeyB(void)
{
	unsigned short ret,len;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	

	if(g_session_key.state!=0xaa)
		return SW_SECURITY_DISSATISFY;

	//len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];

	//len=len/8;
	len=g_APDU.LC-8;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_import_enc_prikey(g_APDU.pInData+2,g_session_key.key,g_session_key.type,g_APDU.pInData+8,len);	
	
}

unsigned short APP_CMD_ImportSM2KeyB1(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x26)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_importprikey(g_APDU.pInData+2,g_APDU.pInData+6);	
	
}

unsigned short APP_CMD_SM2GenZ(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
	}
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		if(flag==1)//使用外部公钥
		{
			cos_SM2genZ(g_APDU.pInData+2+1+g_APDU.pInData[2],g_APDU.pInData[2],g_APDU.pInData+3,g_APDU.pOutData,flag);
		}
		else if(flag==0)//使用内部文件
		{
			cos_SM2genZ(fid,g_APDU.pInData[2],g_APDU.pInData+3,g_APDU.pOutData,flag);
		}			
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2GenKeyA(void)
{
	unsigned short ret;
	unsigned char fid[2], pubid[2],priid[2];
	unsigned char idlen,oidlen,*pid,*poid,*pother_pub,*pother_tmppub,*pin;

	g_APDU.RLE=0;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;

	pin=g_APDU.pInData;

	memcpy(pubid,pin,2);//己方公钥
	pin+=2;
	memcpy(priid,pin,2);//己方私钥
	pin+=2;

	memcpy(g_session_key.keyid,pin,2);//会话密钥ID
	pin+=2;
	g_session_key.type=*pin;//会话密钥类型
	pin+=1;

	pother_pub=pin;//对方公钥
	pin+=64;
	
	pother_tmppub=pin;//对方临时公钥
	pin+=64;

	oidlen=*pin;
	pin+=1;

	poid=pin;
	pin+=oidlen;

	idlen=*pin;
	pin+=1;

	pid=pin;
	pin+=idlen;
	
	ret=cos_open_app(fid,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_SM2genkeya(pubid,priid,idlen,pid,pother_pub,pother_tmppub,oidlen,poid,g_APDU.pOutData,g_session_key.key,0);
		if(ret==SW_SUCCESS)
		{
			g_session_key.state=0xaa;
			g_APDU.RLE=64;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2GenKeyB(void)
{
	unsigned short ret;
	unsigned char fid[2], pubid[2],priid[2];
	unsigned char idlen,oidlen,*pid,*poid,*pother_pub,*pother_tmppub,*pin;

	g_APDU.RLE=0;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;

	pin=g_APDU.pInData;

	memcpy(pubid,pin,2);//己方公钥
	pin+=2;
	memcpy(priid,pin,2);//己方私钥
	pin+=2;

	memcpy(g_session_key.keyid,pin,2);//会话密钥ID
	pin+=2;
	g_session_key.type=*pin;//会话密钥类型
	pin+=1;

	pother_pub=pin;//对方公钥
	pin+=64;
	
	pother_tmppub=pin;//对方临时公钥
	pin+=64;

	oidlen=*pin;
	pin+=1;

	poid=pin;
	pin+=oidlen;

	idlen=*pin;
	pin+=1;

	pid=pin;
	pin+=idlen;
	
	ret=cos_open_app(fid,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_SM2genkeya(pubid,priid,idlen,pid,pother_pub,pother_tmppub,oidlen,poid,g_APDU.pOutData,g_session_key.key,1);
		if(ret==SW_SUCCESS)
		{
			g_session_key.state=0xaa;
			g_APDU.RLE=64;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2EmportKey(void)
{
	unsigned short ret,len;
	unsigned char rsatype;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x44)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	if(g_session_key.state!=0XAA)
		return SW_DATA_NOTFOUND;
	if(g_session_key.keyid[0]!=g_APDU.pInData[2]||g_session_key.keyid[1]!=g_APDU.pInData[3])
		return SW_DATA_NOTFOUND;;
	
	return cos_SM2_exportkey(g_APDU.pInData+4,g_session_key.key,g_APDU.pOutData,&g_APDU.RLE);	
	
}

unsigned short APP_CMD_ImportKey(void)
{
	unsigned short ret,len;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	len=g_APDU.LC-7;

	memcpy(g_session_key.keyid,g_APDU.pInData+4,2);//会话密钥ID
	g_session_key.type=g_APDU.pInData[6];//会话密钥类型
	
	return cos_importkey(g_APDU.pInData+2,g_APDU.pInData+7,len,g_session_key.key);	
	
}

unsigned short APP_CMD_DestoryKey(void)
{
	unsigned short ret;
	unsigned char keyid[2];

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	memset((unsigned char *)g_session_key.state,0,sizeof(SESSION_KEY));

	return SW_SUCCESS;
}
//---------------------------------------------------------------------------

CRYPT_DATA g_crypt;

unsigned short APP_CMD_CryptInit(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x10)
		return SW_LCLE_ERROR;

	if(g_APDU.PP2==ALG_SM4||g_APDU.PP2==ALG_SSF33||g_APDU.PP2==ALG_SM1)
	{
		g_crypt.type=g_APDU.PP2;
		memcpy(g_crypt.key,g_APDU.pInData,16);
		g_crypt.key_sta=0xaa;
	}
	else if(g_APDU.PP2==0x10)
	{
		memcpy(g_crypt.iv,g_APDU.pInData,16);
		g_crypt.iv_sta=0xaa;
	}
	else
		return SW_P1P2_ERROR;

	g_crypt.init=1;
	
	return SW_SUCCESS;
	
}

unsigned short APP_CMD_Encrypt(void)
{
	unsigned short ret;
	unsigned char  cbc_flag,flag=0;
	unsigned char alg_type=0;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2==0)
	{
		cbc_flag=SYMALG_ECB;
	}
	else if(g_APDU.PP2==1)
	{
		cbc_flag=SYMALG_CBC;
		if(g_crypt.iv_sta!=0xaa)
		return SW_CONDITION_DISSATISFY;	
	}
	else
		SW_P1P2_ERROR;

	if(g_APDU.LC%16)
		return SW_LCLE_ERROR;

	if(g_crypt.key_sta!=0xaa)
		return SW_DATA_NOTFOUND;
#if 0	
	memcpy(buf_in,g_APDU.pInData,g_APDU.LC);
	memcpy(key,g_crypt.key,16);
	memcpy(iv,g_crypt.iv,16);
	alg_type=g_crypt.type;
	hal_alg_bcae(buf_in,
				g_APDU.LC, 
				key, 
				iv, 
				SYMALG_SENC|cbc_flag, 
				buf_out,
				alg_type);
	memcpy(g_APDU.pOutData,buf_out,g_APDU.LC);
#else
//	alg_type=g_crypt.type;
////if(g_crypt.init)
//	{
//		hal_alg_bcaeInit(g_crypt.key, 
//					g_crypt.iv,
//					alg_type,
//					SYMALG_SENC|cbc_flag);
//		//g_crypt.init=0;
//	}
//	
//	hal_alg_bcaeCryp(g_APDU.pInData,
//				g_APDU.LC,
//				g_APDU.pOutData,
//				alg_type,
//				flag);

	
	alg_type=g_crypt.type;
	hal_alg_bcae(g_APDU.pInData,
				g_APDU.LC, 
				g_crypt.key, 
				g_crypt.iv, 
				SYMALG_SENC|cbc_flag, 
				g_APDU.pOutData,
				alg_type);

#endif
	g_APDU.RLE=g_APDU.LC;
	
	return SW_SUCCESS;
	
}

unsigned short APP_CMD_Decrypt(void)
{
	unsigned short ret;
	unsigned char cbc_flag,flag=0;
	unsigned char alg_type=0;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2==0)
	{
			cbc_flag=SYMALG_ECB;

	}
	else if(g_APDU.PP2==1)
	{
		cbc_flag=SYMALG_CBC;
		flag=1;
		if(g_crypt.iv_sta!=0xaa)
		return SW_CONDITION_DISSATISFY;
	}
	else
		SW_P1P2_ERROR;

	if(g_APDU.LC%16)
		return SW_LCLE_ERROR;

	if(g_crypt.key_sta!=0xaa)
		return SW_DATA_NOTFOUND;
#if 0
	memcpy(buf,g_APDU.pInData,g_APDU.LC);
	memcpy(key,g_crypt.key,16);
	memcpy(iv,g_crypt.iv,16);
	alg_type=g_crypt.type;
	hal_alg_bcae(buf,
				g_APDU.LC, 
				key, 
				iv, 
				SYMALG_SDEC|cbc_flag, 
				buf,
				alg_type);
	memcpy(g_APDU.pOutData,buf,g_APDU.LC);
	g_APDU.RLE=g_APDU.LC;
#else

//	alg_type=g_crypt.type;
//	if(g_same_cmd==0)
//	{
//		hal_alg_bcaeInit(g_crypt.key, 
//					g_crypt.iv,
//					alg_type,
//					SYMALG_SDEC|cbc_flag);
//	}
//	
//	hal_alg_bcaeCryp(g_APDU.pInData,
//				g_APDU.LC,
//				g_APDU.pOutData,
//				alg_type,
//				flag);

	alg_type=g_crypt.type;
	hal_alg_bcae(g_APDU.pInData,
				g_APDU.LC, 
				g_crypt.key, 
				g_crypt.iv, 
				SYMALG_SDEC|cbc_flag, 
				g_APDU.pOutData,
				alg_type);

	g_APDU.RLE=g_APDU.LC;

#endif
	return SW_SUCCESS;
	
}

unsigned short APP_CMD_Hash(void)
{
	unsigned short ret;
	unsigned char type,len;

	g_APDU.RLE=0;

	if(g_APDU.PP2==HASH_ALG_SM3||g_APDU.PP2==HASH_ALG_SHA256)
	{
		len=32;
	}
	else if(g_APDU.PP2==HASH_ALG_SHA1)
	{
		len=20;
	}
	else if(g_APDU.PP2==HASH_ALG_SHA224)
	{
		len=28;
	}
	else
		return SW_CONDITION_DISSATISFY;
	
	type=g_APDU.PP2;

	if(g_APDU.PP1==0)
	{
		hal_alg_hashinit(type);
		hal_alg_hashupdate(g_APDU.pInData,g_APDU.LC);
		hal_alg_hashfinal(g_APDU.pOutData);
	}
	else if(g_APDU.PP1==1)
	{
		hal_alg_hashinit(type);
		hal_alg_hashupdate(g_APDU.pInData,g_APDU.LC);
		len=0;
	}
	else if(g_APDU.PP1==2)
	{
		hal_alg_hashupdate(g_APDU.pInData,g_APDU.LC);
		len=0;
	}
	else if(g_APDU.PP1==3)
	{
		hal_alg_hashfinal(g_APDU.pOutData);
	}

	else
		SW_P1P2_ERROR;
	
	g_APDU.RLE=len;
	
	return SW_SUCCESS;
	
}

//-----------------------秘钥运算指令END-----------------

//---------------------指纹指令--------------

unsigned char G_XFA_Data[1024*18];
unsigned char *p_G_XFA_Q=G_XFA_Data;
unsigned char *p_G_XFA_C=G_XFA_Data+1;
unsigned int G_XFA_V;
unsigned short APP_CMD_GetFingerInfo(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_getfingerinfo(g_APDU.pOutData,&g_APDU.RLE);
	}	
	return ret;
	
}

unsigned short APP_CMD_ReadFingerAlgInfo(void)
{
	typ_xFA_VersionInfo fingerinfo;
	g_APDU.RLE=0;
	
	if(xFA_ReadVersionInfo(&fingerinfo)==1)
	{
		return SW_SUCCESS;
	}
	else
		return SW_FINGER_ALG_UNLOAD;
	
}
unsigned short APP_CMD_ConnectFinger(void)
{
	unsigned int result,timeout;
	unsigned char counter,sameFinger;
	unsigned short ret;
	int current_counter=0;
	sameFinger=1;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x05)
		return SW_LCLE_ERROR;

	memcpy((unsigned char *)&timeout,g_APDU.pInData,4);
	counter=g_APDU.pInData[4];

	result=xFA_GetImage(timeout);
	
	 if(result==1)//成功
	 {
	 	 result = xFA_ProcessReturnQCV(p_G_XFA_Q, p_G_XFA_C, &G_XFA_V);
	      if (1 == result)
	      {	       
			if (*p_G_XFA_Q < 60 || *p_G_XFA_C < 5 || G_XFA_V < 18000)
	        	{
	         		return SW_FINGER_EIGENVALUEERR;
	       	 }
	       	ret = xFA_AddToTemplate(counter, sameFinger, &current_counter);
		
		g_APDU.RLE=6;
		memcpy(g_APDU.pOutData,(unsigned char *)&current_counter,4);
		memcpy(g_APDU.pOutData+4,(unsigned char *)&ret,2);
	        return SW_SUCCESS;
		}
		else if (0 == result)
	      {	       
	       	 return SW_FINGER_EIGENVALUEERR;
	      }
		else
		{
		    return SW_FINGER_ALG_ERR;
		}
	 }

	else if(result==0)//超时
		return	SW_FINGER_TIMEOUT;
	else 
		return SW_FINGER_CONNETERR;
	
}

unsigned short APP_CMD_GetFinger(void)
{
	unsigned int result;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x04)
		return SW_LCLE_ERROR;


	result=xFA_FingerDetect();

	g_APDU.RLE=4;
	memcpy(g_APDU.pOutData,(unsigned char *)&result,4);
		
	return SW_SUCCESS;
	
}
unsigned short APP_CMD_TemplateInit(void)
{
	unsigned short ret,file_len,offset;
	typ_xFA_VersionInfo finger_alg_info;


	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x00)
		return SW_LCLE_ERROR;

	if(xFA_ReadVersionInfo(&finger_alg_info)==0)
		return SW_FINGER_ALG_UNLOAD;
	if(xFA_TemplateInit()==1)	
		return SW_SUCCESS;
	else
		return SW_FINGER_ALG_UNLOAD;
}

unsigned short APP_CMD_AppendFinger(void)
{
	unsigned short ret,file_len,offset;
	unsigned char buf[33]={0};
	
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x03)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);	
	if(ret==SW_SUCCESS)		
	{	
		if(g_APDU.pInData[2]==0)
			return SW_DATA_PARAERR;//指纹ID不能为0

		if(cos_getsecurestate()!=SECURE_USER)
			return SW_SECURITY_DISSATISFY;
		
		ret=cos_checkfingerstaue();
		if(SW_AUTH_LOCKED!=ret&&SW_FINGER_SPACEFULL!=ret)
		{
			g_APDU.pInData[2]--;
			buf[0]=g_APDU.pInData[2];
			if(xFA_WriteTemplateToAFinger(g_APDU.pInData[2],buf)==1)
			{
				ret=cos_appendfinger(g_APDU.pInData[2]);
			}
				
			else 
				return 0x9204;
		}
		
	}	
	return ret;

	
}
unsigned short APP_CMD_DeleteFinger(void)
{
	unsigned short ret,file_len,offset;
	
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x03)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);	
	if(ret==SW_SUCCESS)		
	{	
		if(cos_getsecurestate()!=SECURE_USER)
			return SW_SECURITY_DISSATISFY;
		ret=cos_checkfingerstaue();
		if(SW_AUTH_LOCKED!=ret&&SW_FUNCTION_NOTSURPORT!=ret)
		{
			if(g_APDU.pInData[2]==0)
			{
				ret=cos_deleteallfinger();
			}
			else
			{
				g_APDU.pInData[2]--;
				if(xFA_EraseAFinger(g_APDU.pInData[2])==1)	
				{
					ret=cos_deletefinger(g_APDU.pInData[2]);
				}
				else
					return 0x9209;
			}				
		}
		
	}
	return ret;
}

unsigned short APP_CMD_MatchFinger(void)
{
	unsigned short ret;
	unsigned int result,timeout;
	unsigned int  sThreshold = 40; // 比对门限，典型值 40，也可30
	unsigned char fingerinfo[16],i,len;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;

	memcpy((unsigned char *)&timeout,g_APDU.pInData+2,4);

	ret=cos_open_app(g_APDU.pInData,0);	
	if(ret==SW_SUCCESS)		
	{
		cos_getfingerinfo(fingerinfo,&len);
		result = xFA_GetImage(timeout);
		if (1 == result)
		{
			result = xFA_ProcessReturnQCV(p_G_XFA_Q, p_G_XFA_C, &G_XFA_V);
			if (1 == result)
			{
				for(i=0;i<fingerinfo[1];i++)
				{
					 result = xFA_MatchFinger(fingerinfo[4+i]-1, sThreshold); // 0xFF 这里指与存储的全部指纹进行比对
					if (1 == result)
					{
						return cos_matchfinger(0);
			     		}
					else if (0 == result)
				      {
				        	if (*p_G_XFA_Q < 60 || *p_G_XFA_C < 5 || G_XFA_V < 18000)
						{
							return SW_FINGER_EIGENVALUEERR;
				        	}
				        	else
				        	{
				        	}
				      }
				      else
				      {			      		
				        	return SW_FINGER_ALG_ERR;
				      }
				}
				
				return cos_matchfinger(1);
		    }
		    else
		    {
				return SW_FINGER_EIGENVALUEERR;
		    }
		}
		else if(0==result)
		{
			return SW_FINGER_TIMEOUT;
		}
		else
			return SW_FINGER_CONNETERR;
	}	
	return ret;	
	
}

unsigned short APP_CMD_DevFormat(void)
{
	g_APDU.RLE=0;

	if(g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	
	if(g_APDU.PP1==0)
	{
		if(g_APDU.LC!=0x60)
			return SW_LCLE_ERROR;
		
		return cos_dev_format(g_APDU.pInData);
	}
	else if(g_APDU.PP1==1)//读设备序列号
	{
		cos_get_devicenum(g_APDU.pOutData);
		g_APDU.RLE=16;
		return SW_SUCCESS;
	}
	else if(g_APDU.PP1==2)//读版本号
	{
		memcpy(g_APDU.pOutData,app_ver,strlen(app_ver));
		g_APDU.RLE=strlen(app_ver);
		return SW_SUCCESS;
	}	
	else
		return SW_P1P2_ERROR;	
}

unsigned short APP_CMD_FactoryCMD(void)
{
	unsigned char buf[16],buf1[16],iv[16];
	unsigned char key[16]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	//unsigned char bufdata[]={0x8b,0x65,0x1a,0x89,0xa7,0x44,0x24,0x41,0x0d,0x0c,0x23,0xe3,0xea,0x4d,0x16,0xec};
	g_APDU.RLE=0;

	if(g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x10)
		return SW_LCLE_ERROR;

	memset(iv,0,16);

	hal_3des_oper(g_APDU.pInData,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);
	
	cos_get_devicenum(buf1);
	if(memcmp(buf,buf1,16))
		return SW_REFERENCEDATA_INVALID;
	
	if(g_APDU.PP1==0)
	{
		g_reboot=1;
	}
	else if(g_APDU.PP1==1)
	{	
		flash_device.erasepage(DEV_VER_INFO);
		flash_device.erasepage(DEV_COS_INIT);
		g_reboot=3;
	}		
	else
		return SW_P1P2_ERROR;	

	return SW_SUCCESS;
}
unsigned short APP_CMD_FingerprintTest(void)
{
	unsigned short ret;
	typ_xFA_VersionInfo finger_alg_info;
	
	unsigned int result,timeout;
	unsigned int  sThreshold = 40; // 比对门限，典型值 40，也可30

	g_APDU.RLE=0;

	if(g_APDU.PP1==0)
	{
		if(g_APDU.LC!=0x04)
		return SW_LCLE_ERROR;
		
		timeout=8000;

		memcpy((unsigned char *)&timeout,g_APDU.pInData,4);

		if(xFA_ReadVersionInfo(&finger_alg_info)==0)
			return SW_FINGER_ALG_UNLOAD;

	//	memcpy(g_APDU.pOutData,(unsigned char *)&finger_alg_info.version,120);
	//	g_APDU.RLE=120;
	//	return SW_SUCCESS;
		
		result = xFA_GetImage(timeout);

		 if(result==1)//成功
		 {
		 	result = xFA_ProcessReturnQCV(p_G_XFA_Q, p_G_XFA_C, &G_XFA_V);
		      if (1 == result)
		      {	       
				if (*p_G_XFA_Q < 60 || *p_G_XFA_C < 5 || G_XFA_V < 18000)
		        	{
		         		return SW_FINGER_EIGENVALUEERR;
		       	 }	
		      		return SW_SUCCESS;
			}
			else if (0 == result)
		      {	       
		       	 return SW_FINGER_EIGENVALUEERR;
		      }
			else
			{
			    return SW_FINGER_ALG_ERR;
			}
		 }

		else if(result==0)//超时
			return	SW_FINGER_TIMEOUT;
		else 
			return SW_FINGER_CONNETERR;
		
	}

	else if(g_APDU.PP1==0xaa)
	{
		if(g_APDU.PP2==0x80)
		{
			hal_alg_genrand(g_APDU.pOutData,256);
			g_APDU.RLE=256;
		}
		
		return SW_SUCCESS;
	}
	else if(g_APDU.PP1==0xbb)
	{		
		hal_alg_genrand(g_APDU.pOutData,1);
		g_APDU.RLE=1;	
		
		return SW_SUCCESS;
	}
	else
		return SW_P1P2_ERROR;	
		
}
//----------------------------指纹指令END----------
#endif

//设备认证
unsigned short APP_CMD_DevAuth(void)
{	
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x10)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;
	
	return cos_dev_auth(g_APDU.pInData,g_rand,g_APDU.PP2);
}

unsigned short APP_CMD_VeryfyPin(void)
{
	unsigned char fid[2];
	unsigned short ret;
	 g_APDU.RLE=0;

	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0x12)
		return SW_LCLE_ERROR;

	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;	
	g_randstate=0;

	ret=cos_open_app(g_APDU.pInData,0);
	
	if(ret==SW_SUCCESS)		
	{
		return cos_veryfypin(g_APDU.pInData+2,g_rand,g_APDU.PP1,g_APDU.PP2);
	}	
	return ret;
}

unsigned short APP_CMD_GenRand(void)
{
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	
	hal_alg_genrand(g_APDU.pOutData,g_APDU.LC);
	memcpy(g_rand,g_APDU.pOutData,8);
	g_randstate=1;
	
	g_APDU.RLE=g_APDU.LC;
	
	return SW_SUCCESS;	 
}

//初始化设备认证密钥
unsigned short APP_CMD_KeyInit(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0x11)
		return SW_LCLE_ERROR;
	
	return cos_init_dev_authkey(g_APDU.pInData);	 
}
//结束初始化
unsigned short APP_CMD_EndInit(void)
{
	DEV_AUTHKEY_INFO *p=(DEV_AUTHKEY_INFO *)DEV_AUTH_KEY;
	U8 flag;
	
	g_APDU.RLE=0;
	flag=0xaa;	
	flash_device.updatedata(DEV_AUTH_KEY,&flag,1);
	
	return SW_SUCCESS; 
}


CMD_TABLE key_cmd[]={
	{0x0050,APP_CMD_DevAuth},
	{0x0051,APP_CMD_VeryfyPin},
	{0x0084,APP_CMD_GenRand},
//	{0x00D8,APP_CMD_MatchFinger},
//	{0x8011,APP_CMD_EndInit},
//	{0x8020,APP_CMD_CreatAPP},
//	{0x8022,APP_CMD_EnumAPP},
//	{0x8026,APP_CMD_OpenAPP},
//	{0x8027,APP_CMD_CloseAPP},
//	{0x8028,APP_CMD_DeleteAPP},	
//	{0x8031,APP_CMD_CryptInit},
//	{0x8032,APP_CMD_Encrypt},
//	{0x8033,APP_CMD_Decrypt},
//	{0x8046,APP_CMD_Gen1024RSA},
//	{0x8047,APP_CMD_Gen2048RSA},
//	{0x8052,APP_CMD_GetPinInfo},
//	{0x8056,APP_CMD_BlockPin},
//	{0x805C,APP_CMD_ClearSecureState},
//	{0x8068,APP_CMD_SetKeyNum},
//	{0x8086,APP_CMD_1024RSADecrypt_Sign},
//	{0x8087,APP_CMD_2048RSADecrypt_Sign},
//	{0x808A,APP_CMD_1024RSAEncrypt_Verify},
//	{0x808B,APP_CMD_2048RSAEncrypt_Verify},
//	{0x80A0,APP_CMD_ImportKey},
//	{0x80A1,APP_CMD_RSAEmportKey},
//	{0x80A2,APP_CMD_SM2EmportKey},
//	{0x80A3,APP_CMD_DestoryKey},
//	{0x80C1,APP_CMD_ExportSM2keyA},
//	{0x80CD,APP_CMD_ImportSM2KeyA},
//	{0x80CB,APP_CMD_ImportSM2KeyB1},
//	{0x80CA,APP_CMD_ReadTerminalInfo},
//	{0x80E1,APP_CMD_CreatFile},
//	{0x80E2,APP_CMD_DeleteFile},
//	{0x80E3,APP_CMD_EnumFile},
//	{0x80E4,APP_CMD_GetFileInfo},
//	{0x80B0,APP_CMD_ReadFile},
//	{0x80B6,APP_CMD_WriteFile},
//	{0x80D1,APP_CMD_AppendFinger},
//	{0x80D2,APP_CMD_DeleteFinger},
//	{0x80D3,APP_CMD_ConnectFinger},
//	{0x80D4,APP_CMD_GetFinger},
//	{0x80D5,APP_CMD_GetFingerInfo},
//	{0x80D7,APP_CMD_TemplateInit},	
//	{0x80F0,APP_CMD_ExportRSAkeyA},
//	{0x80F2,APP_CMD_ImportRSAkeyA},
//	{0x80F3,APP_CMD_ImportRSAkeyB},
//	{0x80F4,APP_CMD_ImportRSAkeyB1},
//	{0x845D,APP_CMD_ChangeDevPin},
//	{0x845E,APP_CMD_ChangePin},
//	{0x845F,APP_CMD_UnlockPin},
//	{0x88CE,APP_CMD_GenSM2Key},
//	{0x88C1,APP_CMD_ExportSM2keyA},
//	{0x88C2,APP_CMD_SM2GenKeyA},
//	{0x88C3,APP_CMD_SM2Sign},
//	{0x88C4,APP_CMD_SM2Verify},
//	{0x88C5,APP_CMD_SM2Decrypt},
//	{0x88C6,APP_CMD_SM2Encrypt},
//	{0x88C8,APP_CMD_SM2GenKeyB},
//	{0x88CB,APP_CMD_ImportSM2KeyB1},
//	{0x80CC,APP_CMD_Hash},
//	{0x88CD,APP_CMD_ImportSM2KeyA},
	{0x9010,APP_CMD_KeyInit},

//	//自定义指令
//	{0x5A00,APP_CMD_DevFormat},
//	{0x5A01,APP_CMD_FactoryCMD},
//	{0x5A02,APP_CMD_FingerprintTest}
};

void Command()
{
	unsigned short  cmd,i,ret;
	unsigned char flag=1;
	static unsigned char former_cmd[4]={0x00};
	
	g_APDU.RLE=0;
	cmd=(g_APDU.CLA<<8)+g_APDU.INS;
	
	for(i = 0; i < sizeof(key_cmd)/sizeof(key_cmd[0]); i++)
	{
		if(cmd== key_cmd[i].cmd)
		{
			ret= key_cmd[i].function();
			flag=0;
			break;
		}
	}
	 if(flag)
	 	ret=0x6d00;
	
	g_APDU.SW1 = ret>>8;
	g_APDU.SW2 = ret&0x00ff;
	return;		//2011.12.20	 		
}

