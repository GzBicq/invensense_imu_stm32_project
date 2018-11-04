#include "stm32f4xx_hal.h"
#include "imu_driver_uart.h"
#include "app_util.h"


#define UART_BAUDRATE       115200

#define UART_RX_GPIO_CLK_ENABLE     __GPIOB_CLK_ENABLE
#define UART_RX_GPIO                GPIOB
#define UART_RX_PIN                 GPIO_PIN_7


#define UART_TX_GPIO_CLK_ENABLE         __GPIOA_CLK_ENABLE
#define UART_TX_GPIO                    GPIOA
#define UART_TX_PIN                     GPIO_PIN_15

UART_HandleTypeDef huart1;



static imu_driver_uart_rxhandler_t m_uart_rxhandler = NULL;
static void *p_m_uart_context = NULL;

static uint8_t m_uart_rx_unit;

static void uart_nvic_init(void)
{
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}


void imu_driver_uart_regester_rxhandler(imu_driver_uart_rxhandler_t uart_rxhandler)
{
    m_uart_rxhandler = uart_rxhandler;
}

void imu_driver_uart_init(void)
{

    huart1.Instance = USART1;
    huart1.Init.BaudRate = UART_BAUDRATE;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    
    uart_nvic_init();

}

void imu_driver_uart_deinit(void)
{
   
    if (HAL_UART_DeInit(&huart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    
    m_uart_rxhandler = NULL;
    p_m_uart_context = NULL;
}

void imu_driver_uart_send_data(uint8_t *p_data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, p_data, len, 50);
}


void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
    
    HAL_UART_Receive_IT(&huart1, &m_uart_rx_unit, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        HAL_UART_Transmit(huart, &m_uart_rx_unit, 1, 50);
        
        if(m_uart_rxhandler)
        {
            m_uart_rxhandler(NULL);
        }
        
    }
}

/* USART1 init callback function, mainly config the specific PIN*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(huart->Instance == USART1)
    {
        UART_RX_GPIO_CLK_ENABLE();
        
        UART_TX_GPIO_CLK_ENABLE();
        
        __HAL_RCC_USART1_CLK_ENABLE();
      
        GPIO_InitStruct.Pin = UART_TX_PIN;
       // GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      //  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(UART_TX_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = UART_RX_PIN;
        HAL_GPIO_Init(UART_RX_GPIO, &GPIO_InitStruct);

    }

}

/* USART1 deinit callback function*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

    if(huart->Instance==USART1)
    {

        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();


        HAL_GPIO_DeInit(UART_TX_GPIO, UART_TX_PIN);
        
        HAL_GPIO_DeInit(UART_RX_GPIO, UART_RX_PIN);
        
        HAL_NVIC_DisableIRQ(USART1_IRQn);

    }

}

