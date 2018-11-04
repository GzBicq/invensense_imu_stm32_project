#ifndef IMU_DRIVER_INT_H_
#define IMU_DRIVER_INT_H_


typedef void (*imu_driver_int_handler_t)(void *p_context);

void imu_driver_int_init(void);

void imu_driver_int_regester_handler(imu_driver_int_handler_t imu_driver_int_handler);

void imu_driver_int_deinit(void);

#endif 