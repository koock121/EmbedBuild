/* STM32F103C8T6 链接脚本（Flash 64K / RAM 20K） */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector : { KEEP(*(.isr_vector)) } > FLASH
  .text : { *(.text*) *(.rodata*) . = ALIGN(4); _etext = .; } > FLASH

  .data : { _sdata = .; *(.data*) . = ALIGN(4); _edata = .; } > RAM AT> FLASH
  _sidata = LOADADDR(.data);

  .bss : { _sbss = .; *(.bss*) *(COMMON) . = ALIGN(4); _ebss = .; } > RAM
}
