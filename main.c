#include <msp430.h> 
#include <stdbool.h>
#define ACLK 0x0100         // Timer_A ACLK source
#define UP 0x0010           // Timer_A UP mode
#define TAIfg 0x0001        // Used to look at Timer A Interrupt Flag

/************************************************************************************
 * Purpose:
 *  	This app exemplifies turning on lights and buttons on a 
 *  	TI MSP430 Microcontroller. This uses Polling, which over utilizes the CPU.
 * Paramaters:
 * 	None
 * Returns:
 * 	None
 * Notes:
 * 	For more information, visit the Texas Instruments webpage, search 'MSP430'
 ************************************************************************************/

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer (WDT)
	PM5CTL0 &= ~LOCKLPM5;       	// Enable GPIO pins (Inputs/ Outputs)

	// Setup clock
	int iCount = 0;             // Used to count 30 seconds and break
	TA0CCR0 = 14043;            // Output compare register for Timer A,
	                            // Number of clock ticks before an interrupt

	TA0CTL = TASSEL__ACLK + MC__UP + ID__8; // settings bound to Timer A
	TA0CTL |= TAIE; 			// more settings

	// Configure GPIO
	P1DIR |= BIT0;              // Direct BIT0 pin as output
	P9DIR |= BIT7;              // Direct BIT1 pin as output
	P1OUT &= ~BIT0;             // Red LED = OFF;
	P9OUT |= BIT7;              // Green LED = ON;

	while(1) { 				// Polling - inefficient
	    if(TA0CTL & TAIfg) { 		// If timer has counted to 40000
	        P1OUT ^= BIT0; 			// Toggle Red LED P1.0
	        P9OUT ^= BIT7; 			// Toggle Green LED P9.7
	        TA0CTL = TA0CTL & (~TAIfg); 	// Count again
	        iCount++;
	        if(iCount == 10){
	            P1OUT &= ~BIT0; 		// Turn LED off
	            P9OUT &= ~BIT7; 		// Turn LED off
	            break;          		// Exit
	        }
	    }
	}

	// Setup input buttons
	P1DIR &= ~BIT1;             // Direct button1 pin as input
	P1DIR &= ~BIT2;             // Direct button2 pin as input

	P1REN = BIT1;               // Enables built-in resistor
	P1REN |= BIT2;

	P1OUT = BIT1;               // Set resistor as pull-up
	P1OUT |= BIT2;

	while(1) {
	    while((BIT1 & P1IN) == 0) {	   // Is P1.1 button pushed?
	        P1OUT |= BIT0;             //   Red LED = ON;
	        P9OUT |= BIT7;             // Green LED = ON;
	    }

	    while((BIT2 & P1IN) == 0) {
	        P1OUT &= ~BIT0;         // LED = OFF;
	        P9OUT &= ~BIT7;         // LED = OFF;
	    }
	}
}
