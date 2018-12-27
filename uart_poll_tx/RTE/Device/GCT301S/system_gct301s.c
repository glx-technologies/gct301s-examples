#include <stdint.h>

#include "gct301s.h"

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

// <h>Code Protection Level
// <o.0..31>SYS_CPL 
//    <i> 0x8602601A : Address 0x0 to 0x1FFF is protected
//    <i> 0xE520C106 : Only SWD_ERA_CMD and SWD_ERA_KEY is allowed
//    <i> All other values from above will allow full access
// </h>
#ifndef SYSTEM_SYS_CPL
#define SYSTEM_SYS_CPL     0x00000000
#endif

// <h>Watchdog Timer Configuration
//
// <q.0>WDT_ON
// <i>Enable the WDT
//
// <o.1..3>Timeout period
//    <0=> 32ms
//    <1=> 128ms
//    <2=> 512ms
//    <3=> 1s
//    <4=> 2s
//    <5=> 4s
//    <6=> 8s
//    <7=> 16s
// <i> Select the WDT timeout period
//
// <q.4>WDT_WARN_EN
// <i>Enable the WDT timeout warning
//
// <q.5>WDT_LPM_EN
// <i>Enable the WDT in SLEEP mode
// 
// <q.6>WDT_DBG_HALT
// <i>Halt the WDT when core is halted 
// 
// </h>
#ifndef SYSTEM_WDT_CFG
#define SYSTEM_WDT_CFG    0x0
#endif

// <h>Chip Bonding Configuration
// <o.0..1>LXTAL selection
//    <0x0=> GC_LXTAL
// <o.2..3>Package selection 
//    <0x0=> 128-pin package
//    <0x1=> 100-pin package
//    <0x3=> 80-pin package
// </h>
#ifndef SYSTEM_CHIP_CFG
#define SYSTEM_CHIP_CFG 0x4
#endif

/*
//-------- <<< end of configuration section >>> ------------------------------
*/

const uint32_t SYS_CPL_VALUE  __attribute__ ((at(0x001000E0),used)) = SYSTEM_SYS_CPL; 
const uint32_t WDT_CFG_VALUE  __attribute__ ((at(0x001000E4),used)) = SYSTEM_WDT_CFG;
const uint32_t CHIP_CFG_VALUE __attribute__ ((at(0x001000E8),used)) = SYSTEM_CHIP_CFG;

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#define XTAL    (4000000UL)            /* Oscillator frequency               */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemFrequency = XTAL;    /*!< System Clock Frequency (Core Clock)  */
uint32_t SystemCoreClock = XTAL;    /*!< Processor Clock Frequency            */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void SystemInit (void)
{
  SystemCoreClock = XTAL;

  SYSCTRL->CLK_SRCSEL = 0;
  SYSCTRL->CLK_DIVSEL = 1;
  return;
}

/**
 * Update the SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Update the SystemCoreClock variable after clock setting changed.
 */
void SystemCoreClockUpdate (void)
{
 SystemCoreClock = XTAL;

}
