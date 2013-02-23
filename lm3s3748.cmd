/******************************************************************************
 *
 * Default Linker Command file for the Texas Instruments LM3S3748
 *
 * This is part of revision 8986 of the Stellaris Peripheral Driver Library.
 *
 *****************************************************************************/

--retain=g_pfnVectors

#define APP_BASE 0x00001800
#define RAM_BASE 0x20000000

MEMORY
{
    FLASH (RX) : origin = APP_BASE, length = 0x0001E800
    SRAM (RWX) : origin = RAM_BASE, length = 0x00010000
}

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=256                                                          */
/* --library=rtsv7M3_T_le_eabi.lib                                           */

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > APP_BASE
    .text   :   > FLASH
    .const  :   > FLASH
    .cinit  :   > FLASH
    .pinit  :   > FLASH

    .vtable :   > RAM_BASE
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM
}

__STACK_TOP = __stack + 512;
