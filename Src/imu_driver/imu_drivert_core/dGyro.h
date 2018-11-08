/**
* @file dGyro.h
* @brief Flash��д����ģ��
* @author 
* @date 
*/

#ifndef _D_GYRO_H_
#define _D_GYRO_H_



#ifdef  __cplusplus
extern "C"
{
#endif
#include "mltypes.h"
  
  
#define D_OK                    0
#define D_ERR                   -1

#define  D_MAX_QUAT_DATA_NUM    4
#define  D_MAX_DIMENSIONALITY   3


/*
 * 定义陀螺仪数据结构体
 */
typedef struct
{
  float       quat_data[D_MAX_QUAT_DATA_NUM];                       //四元数
  float gyro_speed[D_MAX_DIMENSIONALITY];                     //角速度
  float acceleration_speed[D_MAX_DIMENSIONALITY];             //加速度
  float compass[D_MAX_DIMENSIONALITY];                        //磁力计
  float euler_angle[3];                                     //欧拉角
  bool nomotion;
  bool lock_state;
}D_GyroDataTypeDef;



/**
* @brief 陀螺仪驱动初始化
* @retval	 D_OK 初始化成功
* @retval	 D_ERR 初始化失败
*/
int d_gyro_open(void);

/**
* @brief 陀螺仪驱动反初始化
* @retval	 D_OK 反初始化成功
* @retval	 D_ERR 反初始化失败
*/
int d_gyro_close(void);

/**
* @brief 陀螺仪数据读取,将所有数据一次性读取到结构体中
* @retval	 D_OK 读取成功
* @retval	 D_ERR 读取失败
*/
void d_gyro_data_get(D_GyroDataTypeDef *gyro_data);
/**
* @brief   陀螺仪休眠
* @retval  无
*/
void d_gyro_sleep(void);

void handle_input(char c);

void gyro_data_ready_cb(void *p_args);

#ifdef  __cplusplus
}
#endif
#endif

