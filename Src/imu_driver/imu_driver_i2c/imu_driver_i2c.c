#include "stm32f4xx_hal.h"
#include "imu_driver_i2c.h"




#define ANO_RCC_I2C             GPIOB
#define SDA_H                   GPIOB->BSRR=I2C_SDA_GPIO_PIN
#define SCL_H                   GPIOB->BSRR=I2C_SCL_GPIO_PIN
#define SDA_L                   GPIOB->BSRR= (uint32_t) I2C_SDA_GPIO_PIN << 16U
#define SCL_L                   GPIOB->BSRR= (uint32_t) I2C_SCL_GPIO_PIN << 16U
#define SCL_read                GPIOB->IDR  & I2C_SCL_GPIO_PIN
#define SDA_read                GPIOB->IDR  & I2C_SDA_GPIO_PIN

volatile uint8_t        I2C_FastMode;
void I2c_Soft_Init();
uint8_t IIC_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);
uint8_t IIC_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);



int Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr,
                                          unsigned short RegisterLen, unsigned char *RegisterValue)
{
   IIC_Read_nByte(Address, RegisterAddr,RegisterLen, RegisterValue);
   return 0x00;
}


int Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr,
                                           unsigned short RegisterLen, const unsigned char *RegisterValue)
{
    IIC_Write_nByte(Address, RegisterAddr, RegisterLen, (uint8_t *)RegisterValue);
    return 0x00;
}


void I2c_Soft_delay()
{
  __NOP();__NOP();__NOP();
  __NOP();

  if(!I2C_FastMode)
  {
    uint8_t i = 15;
    while(i--);
  }
}


void mpu_i2c_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    I2C_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = I2C_SCL_GPIO_PIN|I2C_SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    
    HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
}

int I2c_Soft_Start()
{
  SDA_H;
  SCL_H;
  I2c_Soft_delay();
  if(!SDA_read)return 0;  
  SDA_L;
  I2c_Soft_delay();
  if(SDA_read) return 0;  
  SDA_L;
  I2c_Soft_delay();
  return 1;

}

void I2c_Soft_Stop()
{
  SCL_L;
  I2c_Soft_delay();
  SDA_L;
  I2c_Soft_delay();
  SCL_H;
  I2c_Soft_delay();
  SDA_H;
  I2c_Soft_delay();
}

void I2c_Soft_Ask()
{
  SCL_L;
  I2c_Soft_delay();
  SDA_L;
  I2c_Soft_delay();
  SCL_H;
  I2c_Soft_delay();
  SCL_L;
  I2c_Soft_delay();
}

void I2c_Soft_NoAsk()
{
  SCL_L;
  I2c_Soft_delay();
  SDA_H;
  I2c_Soft_delay();
  SCL_H;
  I2c_Soft_delay();
  SCL_L;
  I2c_Soft_delay();
}

int I2c_Soft_WaitAsk(void)   
{
  uint8_t ErrTime = 0;
  SCL_L;
  I2c_Soft_delay();
  SDA_H;
  I2c_Soft_delay();
  SCL_H;
  I2c_Soft_delay();
  while(SDA_read)
  {
    ErrTime++;
    if(ErrTime>50)
    {
      I2c_Soft_Stop();
      return 1;
    }
  }
  SCL_L;
  I2c_Soft_delay();
  return 0;
}

void I2c_Soft_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        I2c_Soft_delay();
      if(SendByte&0x80)
        SDA_H;
      else
        SDA_L;
        SendByte<<=1;
        I2c_Soft_delay();
        SCL_H;
        I2c_Soft_delay();
    }
    SCL_L;
}

uint8_t I2c_Soft_ReadByte(uint8_t ask) 
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;
    while(i--)
    {
      ReceiveByte<<=1;
      SCL_L;
      I2c_Soft_delay();
      SCL_H;
      I2c_Soft_delay();
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;

  if (ask)
    I2c_Soft_Ask();
  else
    I2c_Soft_NoAsk();
    return ReceiveByte;
}



uint8_t IIC_Write_1Byte(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
  I2c_Soft_Start();
  I2c_Soft_SendByte(SlaveAddress<<1);
  if(I2c_Soft_WaitAsk())
  {
    I2c_Soft_Stop();
    return 1;
  }
  I2c_Soft_SendByte(REG_Address);
  I2c_Soft_WaitAsk();
  I2c_Soft_SendByte(REG_data);
  I2c_Soft_WaitAsk();
  I2c_Soft_Stop();
  return 0;
}


uint8_t IIC_Read_1Byte(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t *REG_data)
{
  I2c_Soft_Start();
  I2c_Soft_SendByte(SlaveAddress<<1);
  if(I2c_Soft_WaitAsk())
  {
    I2c_Soft_Stop();
    return 1;
  }
  I2c_Soft_SendByte(REG_Address);
  I2c_Soft_WaitAsk();
  I2c_Soft_Start();
  I2c_Soft_SendByte(SlaveAddress<<1 | 0x01);
  I2c_Soft_WaitAsk();
  *REG_data= I2c_Soft_ReadByte(0);
  I2c_Soft_Stop();
  return 0;
}

uint8_t IIC_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf)
{
  I2c_Soft_Start();
  I2c_Soft_SendByte(SlaveAddress<<1);
  if(I2c_Soft_WaitAsk())
  {
    I2c_Soft_Stop();
    return 1;
  }
  I2c_Soft_SendByte(REG_Address);
  I2c_Soft_WaitAsk();
  while(len--)
  {
    I2c_Soft_SendByte(*buf++);
    I2c_Soft_WaitAsk();
  }
  I2c_Soft_Stop();
  return 0;
}

uint8_t IIC_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf)
{
  I2c_Soft_Start();
  I2c_Soft_SendByte(SlaveAddress<<1);
  if(I2c_Soft_WaitAsk())
  {
    I2c_Soft_Stop();
    return 1;
  }
  I2c_Soft_SendByte(REG_Address);
  I2c_Soft_WaitAsk();

  I2c_Soft_Start();
  I2c_Soft_SendByte(SlaveAddress<<1 | 0x01);
  I2c_Soft_WaitAsk();
  while(len)
  {
    if(len == 1)
    {
      *buf = I2c_Soft_ReadByte(0);
    }
    else
    {
      *buf = I2c_Soft_ReadByte(1);
    }
    buf++;
    len--;
  }
  I2c_Soft_Stop();
  return 0;
}
