#ifndef	__HAL_FLASH_INTERFACE_H__
#define	__HAL_FLASH_INTERFACE_H__

extern struct hw_flash_device flash_device;

//------------------------�ṹ�嶨��------------------------------
struct hw_flash_device
{
	unsigned char (*init)(void);
	unsigned char (*writedata)(unsigned int addr,unsigned char *pin,unsigned short len);
	unsigned char (*updatedata)(unsigned int addr,unsigned char *pout,unsigned short len);
	unsigned char (*readdata)(unsigned int addr,unsigned char *pout,unsigned short len);
	unsigned char (*erasepage)(unsigned int addr);
	unsigned char (*updateflag)(unsigned int addr,unsigned char status);//������ֻ����һ���ֽ�
};

#endif

