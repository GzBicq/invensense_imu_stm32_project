#include "imu_driver_uart.h"
#include "app_util.h"


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
    char sprintf_buf[50] = "";
    uint16_t len = 0;
    len = sprintf(sprintf_buf, "_ERROR File: %s,line: %d ", file, line);
    imu_driver_uart_send_data(sprintf_buf, len);
}
