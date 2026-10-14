/* STM32F103 最小示例：寄存器点灯（PC13），符号均来自 CMSIS 设备头 */
#include "stm32f103xb.h"

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~(0xFUL << 20);
    GPIOC->CRH |=  (0x2UL << 20);          /* PC13 推挽输出 2MHz */

    for (;;) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        for (volatile int i = 0; i < 200000; i++) __asm__("nop");
    }
}
