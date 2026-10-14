/* 最小启动文件：向量表 + Reset_Handler */
#include "stdint.h"

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);
void Reset_Handler(void);
void Default_Handler(void) { for (;;); }

extern void NMI_Handler(void)     __attribute__((weak, alias("Default_Handler")));
extern void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));

__attribute__((section(".isr_vector")))
const void *g_vectors[] = {
    &_estack, Reset_Handler,
    NMI_Handler, HardFault_Handler,
};

void Reset_Handler(void)
{
    uint32_t *src = &_sidata, *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;
    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;
    main();
    for (;;);
}
