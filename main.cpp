#include <stm32h7xx_hal.h>
#include <stm32_hal_legacy.h>

extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
extern "C" void SVC_Handler(uint8_t code)
{	
	switch (code)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}

// Useless if executed not in interrupt AND after disabling of privileged mode
void SetPrivilegedMode(bool enabled)
{
	uint32_t ctrl = __get_CONTROL();
	if (enabled)
		ctrl &= ~0x1;
	else
		ctrl |= 0x1;
	__set_CONTROL(ctrl);
}

void PendSvc(uint8_t code)
{
	asm("svc 1");
}

int main(void)
{
	HAL_Init();
	__GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	for (;;)
	{
		PendSvc(1);
		HAL_Delay(500);
		PendSvc(2);
		HAL_Delay(500);
	}
}
