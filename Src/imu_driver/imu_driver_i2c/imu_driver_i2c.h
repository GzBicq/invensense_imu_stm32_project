#ifndef _IMU_DRIVER_I2C_H_
#define _IMU_DRIVER_I2C_H_


#define SENSORS_I2C                     I2C2

#define I2C_OWN_ADDRESS                 0x00

#define I2C_Config()                    I2cMaster_Init();


#define I2C_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C_SCL_GPIO                    GPIOB
#define I2C_SDA_GPIO                    GPIOB
#define I2C_SCL_GPIO_PIN                GPIO_PIN_6
#define I2C_SDA_GPIO_PIN                GPIO_PIN_7



int Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, 
                                          unsigned short RegisterLen, unsigned char *RegisterValue);
int Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, 
                                           unsigned short RegisterLen, const unsigned char *RegisterValue);


void mpu_i2c_init(void);

void mpu_i2c_deinit(void);

#endif//_INT_H_