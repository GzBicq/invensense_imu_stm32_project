#ifndef IMU_DRIVER_UART_H_
#define IMU_DRIVER_UART_H_

#include "stdint.h"

typedef void(* imu_driver_uart_rxhandler_t)(void *p_context);

void imu_driver_uart_init(void);

void imu_driver_uart_deinit(void);

void imu_driver_uart_send_data(uint8_t *p_data, uint16_t len);

void imu_driver_uart_regester_rxhandler(imu_driver_uart_rxhandler_t uart_rxhandler);


#endif