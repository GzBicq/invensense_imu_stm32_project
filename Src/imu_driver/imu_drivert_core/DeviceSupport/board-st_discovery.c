#include "stm32f4xx_hal.h"
#include "board-st_discovery.h"


volatile uint32_t g_ul_ms_ticks = 0;
static volatile uint32_t TimingDelay=0;


void mdelay(unsigned long nTime)
{
        TimingDelay = nTime;
        while(TimingDelay != 0);
}

int get_tick_count(unsigned long *count)
{
        count[0] = g_ul_ms_ticks;
        return 0;
}

void TimingDelay_Decrement(void)
{
        if (TimingDelay != 0x00)
		TimingDelay--;
}

void TimeStamp_Increment(void)
{
        g_ul_ms_ticks++;
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
      TimingDelay_Decrement();
      TimeStamp_Increment();
      HAL_IncTick();
      HAL_SYSTICK_IRQHandler();
}
