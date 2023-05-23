#include <LPC17xx.h>

void delay(unsigned int r1);
void UART0_Init(void);
void UART0_IRQHandler(void);

unsigned long int r = 0, i = 0;
unsigned char tx0_flag = 0;
unsigned char *ptr, arr[] = "Hello world";

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    UART0_Init();

    while (1)
    {
        ptr = arr;

        while (*ptr != '\0')
        {
            LPC_UART0->THR = *ptr++;
            while (tx0_flag == 0x00);
            tx0_flag = 0x00;
            for (i = 0; i < 200; i++);
        }

        for (i = 0; i < 500; i++)
            delay(625); // Delay
    }
}

void UART0_Init(void)
{
    LPC_SC->PCONP |= 0x00000008;     // UART0 peripheral enable
    LPC_PINCON->PINSEL0 |= 0x00000050;   // Selecting TX0[P0.2-->5:4] and RX0[P0.3-->7:6] of UART0
    LPC_UART0->LCR = 0x00000083;    // Enable divisor latch, parity disable, 1 stop bit, 8-bit word length line control register
    LPC_UART0->DLM = 0X00;
    LPC_UART0->DLL = 0x13;  // Select baud rate 9600 bps
    LPC_UART0->LCR = 0X00000003;
    LPC_UART0->FCR = 0x07;
    LPC_UART0->IER = 0X03;  // Select Transmit and receive interrupt
    NVIC_EnableIRQ(UART0_IRQn);  // Assigning channel
}

void UART0_IRQHandler(void)
{
    unsigned long Int_Stat;
    Int_Stat = LPC_UART0->IIR;  // Reading the data from interrupt identification register
    Int_Stat = Int_Stat & 0x06;  // Masking other than transmit int & receive data indicator

    if ((Int_Stat & 0x02) == 0x02)  // Transmit interrupt
        tx0_flag = 0xff;
}

void delay(unsigned int r1)
{
    for (r = 0; r < r1; r++);
}
