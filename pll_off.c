#include <LPC17xx.h>

#define CCLKCFG (*(volatile unsigned char *)(0x400FC104))
#define PLL0CON (*(volatile unsigned char *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned char *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned char *)(0x400FC088))

// Function prototypes
void delay(void);

int main() {
    LPC_GPIO2->FIODIR |= 0x0000007C;
    CCLKCFG = 0x000000FF;

    // Disconnect PLL0
    PLL0CON &= !(1 << 1); // Clears bit 1 of PLL0CON, the Connect bit
    PLL0FEED = 0xAA; // Feed the PLL. Enables action of the above line
    PLL0FEED = 0x55;
    
    // Wait for PLL0 to disconnect. Wait for bit 25 to become 0.
    while ((PLL0STAT & (1 << 25)) != 0x00); // Bit 25 shows connection status

    // Turn off PLL0; on completion, PLL0 is bypassed.
    PLL0CON &= !(1 << 0); // Bit 0 of PLL0CON disables PLL
    PLL0FEED = 0xAA; // Feed the PLL. Enables action of the above line
    PLL0FEED = 0x55;

    // Wait for PLL0 to shut down
    while ((PLL0STAT & (1 << 24)) != 0x00); // Bit 24 shows enable status

    /**** Insert Optional Extra Code Here ****
    to change PLL0 settings or clock source.
    ** OR ** just continue with PLL0 disabled and bypassed */

    // Blink at the new clock frequency
    while (1) {
        LPC_GPIO2->FIOPIN ^= (0x0000007C);
        delay();
    }
}

void delay(void) {
    // Delay function.
    int j; // Loop variable j
    for (j = 0; j < 5000000; j++) {
        j++;
        j--; // Waste time
    }
}
