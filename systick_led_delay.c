#include <LPC17xx.h> 

/* Systick Register address, refer datasheet for more info */
#define STCTRL   (*((volatile unsigned long *) 0xE000E010))
#define STRELOAD (*((volatile unsigned long *) 0xE000E014))
#define STCURR   (*((volatile unsigned long *) 0xE000E018))

/*******STCTRL bits*******/
#define SBIT_ENABLE    0
#define SBIT_TICKINT   1
#define SBIT_CLKSOURCE 2

/* 100000000Mhz * 1ms = 1000000 - 1 */
#define RELOAD_VALUE 99999999
#define LED 2 // P2_2

int main(void) {
    SystemInit();

    STRELOAD = RELOAD_VALUE; // Set reload value for 100ms tick

    /* Enable the Systick, Systick Interrupt, and select CPU Clock Source */
    STCTRL = (1 << SBIT_ENABLE) | (1 << SBIT_TICKINT) | (1 << SBIT_CLKSOURCE);

    LPC_GPIO2->FIODIR = (1 << LED); /* Configure the LED Pin as Output */

    while (1) {
        // Do nothing
    }
}

void SysTick_Handler(void) {
    LPC_GPIO2->FIOPIN ^= (1 << LED); /* Toggle the LED (P2_2) */
}
