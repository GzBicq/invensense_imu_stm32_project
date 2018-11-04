#include "stm32f4xx_hal.h"
#include "imu_driver_int.h"


static imu_driver_int_handler_t m_imu_driver_int_handler = NULL;



static void int_nvic_init(void)
{
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void imu_driver_int_regester_handler(imu_driver_int_handler_t imu_driver_int_handler)
{
    m_imu_driver_int_handler = imu_driver_int_handler;
}

void imu_driver_int_init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  int_nvic_init();

}


void imu_driver_int_deinit(void)
{
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
    
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    
    m_imu_driver_int_handler = NULL;
}

void EXTI9_5_IRQHandler(void)
{

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    m_imu_driver_int_handler(NULL);
}
