#include <LPC17xx.h>

#define CCLKCFG (*(volatile unsigned long *)(0x400FC104))
#define PLL0CON (*(volatile unsigned long *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned long *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned long *)(0x400FC088))
#define PLL0CFG (*(volatile unsigned long *)(0x400FC084))

// Function prototypes
void delay(void);

int main() {
    LPC_GPIO2->FIODIR |= 0x0000007C;

    // CCLKCFG=0x000000EE; // divider divides by this number plus 1

    // Set PLL0 multiplier
    PLL0CFG = 0x0015013A; // Arbitrary multiply value, divide value left at 1
    PLL0FEED = 0x000000AA; // Feed the PLL
    PLL0FEED = 0x00000055;

    // Turn on PLL0
    PLL0CON |= 1 << 0;
    PLL0FEED = 0x000000AA; // Feed the PLL
    PLL0FEED = 0x00000055;

    // Wait for main PLL (PLL0) to come up
    while ((PLL0STAT & (1 << 24)) == 0x00);

    // Wait for PLOCK0 to become 1
    while ((PLL0STAT & (1 << 26)) == 0x00);

    // Connect to the PLL0
    PLL0CON |= 1 << 1;
    PLL0FEED = 0x000000AA; // Feed the PLL
    PLL0FEED = 0x00000055;

    while ((PLL0STAT & (1 << 25)) == 0x00); // Wait for PLL0 to connect

    while (1) {
        LPC_GPIO2->FIOPIN ^= (0x0000007C);
        delay();
    }
}

void delay(void) {
    // Delay function.
    int j; // Loop variable j
    for (j = 0; j < 50000; j++) {
        j++;
        j--; // Waste time
    }
}
