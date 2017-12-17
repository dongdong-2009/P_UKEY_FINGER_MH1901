
//#include"mhscpu.h"
#include"mhscpu_lcdi.h"




//LCD module
#define CTRL_AUTO_READ       (1 << 8)
#define CTRL_TYPE_8080       (1 << 7)
#define CTRL_EN_RD           (1 << 6)
#define CTRL_RW_WR           (1 << 5)
#define CTRL_CD              (1 << 4)
#define CTRL_WIDTH_4BIT      (1 << 3)
#define CTRL_AUTO            (1 << 2)
#define CTRL_RD_IE           (1 << 1)
#define CTRL_WR_IE           (1 << 0)

#define STATUS_READY         (1 << 2)
#define STATUS_RD_IS         (1 << 1)
#define STATUS_WR_IS         (1 << 0)


typedef struct
{
    uint32_t volatile u32Head;
    uint32_t volatile u32Tail;
    uint32_t volatile u32BuffSize;
    uint32_t volatile u32Stat;
    uint8_t volatile *pu8Buff;

} RNG_BUF8, *RNG_BUF8_ID;

typedef struct
{
    uint32_t volatile u32Head;
    uint32_t volatile u32Tail;
    uint32_t volatile u32BuffSize;
    uint32_t volatile u32Stat;
    uint32_t volatile *pu32Buff;

} RNG_BUF32, *RNG_BUF32_ID;

#define RNG_BUF_IS_FULL(pBuf) ((pBuf)->u32Tail == (pBuf)->u32Head)
#define RNG_BUF_NEXT_HEAD(pBuf) (((pBuf)->u32Head + 1) % ((pBuf)->u32BuffSize))
#define RNG_BUF_NEXT_TAIL(pBuf) (((pBuf)->u32Tail + 1) % ((pBuf)->u32BuffSize))
#define RNG_BUF_LEN(pBuf) (((pBuf)->u32Tail + (pBuf)->u32BuffSize - 1 - (pBuf)->u32Head) % (pBuf)->u32BuffSize)
#define RNG_BUF_PERI_TAIL(pBuf) (((pBuf)->u32Tail + (pBuf)->u32BuffSize - 1) % ((pBuf)->u32BuffSize))



#define MH_SMCU_INT_SET_ENABLE_BASE     (0xE000E100UL)
#define MH_SMCU_INT_CLEAR_ENABLE_BASE   (0xE000E180UL)

#define MH_SMCU_INT_SET_PENDING_BASE	(0xE000E200UL)
#define MH_SMCU_INT_CLEAR_PENDING_BASE	(0xE000E280UL)
#define MH_SMCU_INT_ACTIVE_BIT_BASE	    (0xE000E300UL)

#define MH_SMCU_SOFT_TRIG_INT_REG_BASE  (0xE000EF00UL)



//static uint8_t u8Mode = MODE_8080;

static uint32_t u32OpCount = 0;
static RNG_BUF8 rbRead = {0, 0, 1, 0, 0}, rbWrite = {0, 0, 1, 0, 0}; 
static RNG_BUF32 rbCmd = {0, 0, 1, 0, 0};

static RNG_BUF32_ID prbCmd = &rbCmd;
static RNG_BUF8_ID prbRead = &rbRead, prbWrite = &rbWrite; 

#define BIT(n)          (1UL << (n))

void LCD_BusRead(uint8_t u8CD)
{
    //uint8_t u8Value = LCD->lcdi_data;

    if (CTRL_TYPE_8080 != (CTRL_TYPE_8080 & LCD->lcdi_ctrl))
    {
        //In 6800 mode we should set /WR signal manually.
        LCD->lcdi_ctrl |= CTRL_RW_WR;
    }

    //Set CD(A0) signal.0:Instrcution; 1:Data)
    if (CMD == u8CD)
    {
        LCD->lcdi_ctrl &= ~CTRL_CD;
    }
    else
    {
        LCD->lcdi_ctrl |= CTRL_CD;
    }
    LCD->lcdi_ctrl |= CTRL_AUTO_READ;

    //return u8Value;
}

void LCD_BusWrite(uint8_t u8CD, uint8_t value)
{
    if (CTRL_TYPE_8080 != (CTRL_TYPE_8080 & LCD->lcdi_ctrl))
    {
        //In 6800 mode we should set /WR signal manually.
        LCD->lcdi_ctrl &= ~CTRL_RW_WR;
    }

    //Set CD(A0) signal.0:Instrcution; 1:Data)
    if (CMD == u8CD)
    {
        LCD->lcdi_ctrl &= ~CTRL_CD;
    }
    else
    {
        LCD->lcdi_ctrl |= CTRL_CD;
    }
    //Write to lcdi_data will trigger write timing.
    LCD->lcdi_data = value;
}


void LCD_IRQHandler(void)
{
    uint8_t u8Cmd, u8RW, u8Data;
    uint32_t u32Next, u32Count, u32Operate, u32Len;

    //LCD interrupt can be set manually so we should wait LCD real done.
    while (STATUS_READY != (LCD->lcdi_status & STATUS_READY));

    u32Len = RNG_BUF_LEN(prbCmd);
    if (0 == u32Len)
    {
        //No comamnd
        u32OpCount = LCD->lcdi_data;
        //LCD->lcdi_ctrl &= ~STATUS_WR_IS;
        u32OpCount = 0;
        return;
    }

    u32Next = RNG_BUF_NEXT_HEAD(prbCmd);
    u32Operate = prbCmd->pu32Buff[u32Next];
    u8RW = BIT(31) == (u32Operate & BIT(31));
    u8Cmd = BIT(30) == (u32Operate & BIT(30));
    u32Count = u32Operate & ~(BIT(30) | BIT(31));
    if (u32Count > 0)
    {
        //BIT31 read or write, 0:write, 1:read
        if (u8RW)
        {
            u8Data = LCD->lcdi_data;
            if (!RNG_BUF_IS_FULL(prbRead))
            {
                prbRead->pu8Buff[prbRead->u32Tail] = u8Data;
                if (u32OpCount > 0)
                {
                    //Skip first data
                    prbRead->u32Tail = RNG_BUF_NEXT_TAIL(prbRead);
                }
                if (u32OpCount < u32Count)
                {
                    u32OpCount++;
                    LCD_BusRead(u8Cmd);
                }
                else
                {
                    prbCmd->u32Head = u32Next;
                    u32OpCount = 0;
                }
            }
            else
            {
                prbRead->u32Stat = 1;
                prbCmd->u32Head = u32Next;
                u32OpCount = 0;
            }
        }
        else
        {
            if (0 != RNG_BUF_LEN(prbWrite) && u32OpCount < u32Count)
            {
                u32OpCount++;
                prbWrite->u32Head = RNG_BUF_NEXT_HEAD(prbWrite);
                LCD_BusWrite(u8Cmd, prbWrite->pu8Buff[prbWrite->u32Head]);
            }
            else
            {
                prbCmd->u32Head = u32Next;
                u32OpCount = 0;
            }
        }
    }

    if (prbCmd->u32Head == u32Next && 0 == u32OpCount)
    {
        if (u32Len > 1)
        {
            NVIC_SetPendingIRQ(LCD_IRQn);
        }
    }
}

static void LCD_SetInt(uint8_t u8RW, uint8_t u8Enable)
{
    uint32_t u32Bit;
    if (u8RW)
    {
        //Set read
        u32Bit = CTRL_RD_IE;
    }
    else
    {
        //Set write
        u32Bit = CTRL_WR_IE;

    }
    if (u8Enable)
    {
        while (0 == (LCD->lcdi_ctrl & u32Bit))
        {
            LCD->lcdi_ctrl |= u32Bit;
        }
    }
    else
    {
        while (LCD->lcdi_ctrl & u32Bit)
        {
            LCD->lcdi_ctrl &= ~u32Bit;
        }
    }
}

//Write with no pendding.
int32_t LCD_WriteBuff(uint8_t opt, uint8_t *pu8Buff, uint32_t u32BuffLen)
{
    uint32_t i, u32Cmd, u32Len = RNG_BUF_LEN(prbWrite);

    if (u32BuffLen > BIT(30) || RNG_BUF_IS_FULL(prbCmd) || prbWrite->u32BuffSize < u32Len + u32BuffLen + 1)
    {
        return -1;
    }
    for (i = 0; i < u32BuffLen; i++)
    {
        prbWrite->pu8Buff[prbWrite->u32Tail] = pu8Buff[i];
        prbWrite->u32Tail = RNG_BUF_NEXT_TAIL(prbWrite);
    }

    u32Cmd = u32BuffLen;
    if (CMD != opt)
    {
        u32Cmd |= BIT(30);
    }
    //Put cmd to queue.
    prbCmd->pu32Buff[prbCmd->u32Tail] = u32Cmd;
    prbCmd->u32Tail = RNG_BUF_NEXT_TAIL(prbCmd);

    //LCD->lcdi_ctrl |= CTRL_WR_IE;
    LCD_SetInt(0, 1);
    //LCD can't generate int at first time so we pend write interrupt manually.
    NVIC_SetPendingIRQ(LCD_IRQn);
	
	return u32BuffLen;
}

//Read with pendding.
int32_t LCD_ReadBuff(uint8_t opt, uint8_t *pu8Buff, uint32_t u32BuffLen)
{
    uint32_t i, u32Cmd, u32Len = RNG_BUF_LEN(prbRead);

    if (u32BuffLen > BIT(30) || RNG_BUF_IS_FULL(prbCmd) || prbRead->u32BuffSize < u32Len + u32BuffLen + 1)
    {
        return -1;
    }

    u32Cmd = u32BuffLen | BIT(31);
    if (CMD != opt)
    {
        u32Cmd |= BIT(30);
    }

    //Must wait for other operation complate or maybe lost interrupt. 
    //while (0 != RNG_BUF_LEN(prbCmd));
    //Put cmd to queue.
    //posi = RNG_BUF_PERI_TAIL(prbRead);
    //i = posi < prbCmd->u32Head;
    prbCmd->pu32Buff[prbCmd->u32Tail] = u32Cmd;
    prbCmd->u32Tail = RNG_BUF_NEXT_TAIL(prbCmd);

    LCD_SetInt(1, 1);
    //LCD->lcdi_ctrl |= CTRL_RD_IE;
    //LCD can't generate int at first time so we pend write interrupt manually.
    NVIC_SetPendingIRQ(LCD_IRQn);
    
    for (i = 0; i < u32BuffLen; i++)
    {
        while (0 == RNG_BUF_LEN(prbRead));
        prbRead->u32Head = RNG_BUF_NEXT_HEAD(prbRead);
        pu8Buff[i] = prbRead->pu8Buff[prbRead->u32Head];
    }
	
	return u32BuffLen;
}



uint8_t lcd_ReadBuff[LCD_READ_BUFF_SIZE];
uint8_t lcd_WriteBuff[LCD_WRITE_BUFF_SIZE];
uint32_t lcd_CmdBuff[LCD_CMD_BUFF_SIZE];

static void LCD_BuffInit(void)
{
    rbRead.u32Head = 0;
    rbRead.u32Tail = 1;
    rbRead.u32BuffSize = LCD_READ_BUFF_SIZE;
    rbRead.pu8Buff = lcd_ReadBuff;

    rbWrite.u32Head = 0;
    rbWrite.u32Tail = 1;
    rbWrite.u32BuffSize = LCD_WRITE_BUFF_SIZE;
    rbWrite.pu8Buff = lcd_WriteBuff;

    rbCmd.u32Head = 0;
    rbCmd.u32Tail = 1;
    rbCmd.u32BuffSize = LCD_CMD_BUFF_SIZE;
    rbCmd.pu32Buff = lcd_CmdBuff;

    u32OpCount = 0;
}

/*
__STATIC_INLINE uint32_t NVIC_GetEnableIRQ(IRQn_Type IRQn)
{
    return((uint32_t) ((NVIC->ISER[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0)); 
}
*/
void LCD_Init(LCD_InitTypeDef *pLcdInit)
{
    uint32_t u32Cycle;
    NVIC_ClearPendingIRQ(LCD_IRQn);
    SYSCTRL->SOFT_RST2 |= BIT(1);
	LCD_BuffInit();

    //Set mode and auto read
    if (MODE_8080 == pLcdInit->LCD_BusMode)
    {
        LCD->lcdi_ctrl = CTRL_TYPE_8080 | CTRL_AUTO;
    }
    else
    {
        //In 6800 mode we should set /WR signal manually.
        LCD->lcdi_ctrl = CTRL_EN_RD | CTRL_AUTO;
    }

//#define HCLK_1MS                    (*(volatile uint32_t *)(0x40000024))

	//0.1u * 54Mhz = 5.4
    //LCD->lcdi_cycle = (((SYSCTRL->PCLK_1MS_VAL >> 1) / 1000 + 500) / 1000 * pLcdInit->LCD_MaxQTR + 500) / 1000;
    //LCD->lcdi_cycle = (((SYSCTRL->PCLK_1MS_VAL >> 1) / 1000 + 500) / 1000 * pLcdInit->LCD_MaxQTR + 500) / 1000;
    u32Cycle = (SYSCTRL->HCLK_1MS_VAL * pLcdInit->LCD_MaxQTR + 500000) / 1000000;
    if (u32Cycle > 1)
    {
        u32Cycle -= 1;
    }
    LCD->lcdi_cycle = u32Cycle;
    if (pLcdInit->LCD_IntRead)
    {
        LCD->lcdi_ctrl |= CTRL_RD_IE;
    }
    if (pLcdInit->LCD_IntWrite)
    {
        LCD->lcdi_ctrl |= CTRL_WR_IE;
    }
}


void LCD_Write(uint8_t u8CD, uint8_t u8Value)
{

    LCD_BusWrite(u8CD, u8Value);
    while (STATUS_READY != (LCD->lcdi_status & STATUS_READY));

/*	
    if (MODE_8080 != u8Mode)
    {
        //In 6800 mode switch the /WR signal to high.
        LCD->lcdi_ctrl |= CTRL_RW_WR;
    }

    if (CMD == opt)
    {
        LCD->lcdi_ctrl |= CTRL_CD;
    }
    else
    {
        LCD->lcdi_ctrl &= ~CTRL_CD;
    }
*/
}


void LCD_Read(uint8_t u8CD, uint8_t *dat)
{
    LCD_BusRead(u8CD);
    while (STATUS_READY != (LCD->lcdi_status & STATUS_READY));
	*dat = LCD->lcdi_data;
/*
    if (MODE_8080 != u8Mode)
    {
        //In 6800 mode switch the RD signal to low.
        LCD->lcdi_ctrl &= ~CTRL_RW_WR;
    }

    //Swtich A0 signal.
    if (CMD == opt)
    {
        LCD->lcdi_ctrl |= CTRL_CD;
    }
    else
    {
        LCD->lcdi_ctrl &= ~CTRL_CD;
    }
*/
}

