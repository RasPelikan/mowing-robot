#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>
#include <stdbool.h>

/*
 * Enables the real time clock
 */
void enable_rtc() {

	/*
	 * Wait for external clock crystal to stabilize
	 */
	int temp0, temp1;
	for (temp0 = 0; temp0 < 0x0040; temp0++) {
		for (temp1 = 0; temp1 < 0xFFFF; temp1++);
	}

	/*
	 * Setup timer 2
	 */
	// Disable timer2 (if not yet disabled)
	TIMSK2 &= ~(_BV(TOIE2) | _BV(OCIE2A) | _BV(OCIE2B));
	//power_timer2_disable();
	ASSR &= ~(_BV(EXCLK));			// Disable external clock since we use internal clock for CPU
	ASSR |= _BV(AS2);				// Set timer2 to be asynchronous from the CPU clock
	TCNT2 = 0x00;					// Clear timer-counter
	// Set prescaler to 256 (= 8 bit) * 128 (= prescaler) = 32768 kHz = 1s
	TCCR2B = _BV(CS20) | _BV(CS22);
	// Wait until busy-flag changes to is gone
	while (ASSR & (_BV(TCN2UB) | _BV(TCR2BUB)));
	//power_timer2_enable();			// Enable timer2 while sleeping
	TIMSK2 = _BV(TOIE2);			// Enable timer2 overflow interrupt

}

/*
 * Necessary action after wake up from sleep for asynchronous timer2
 */
void rtc_wakupe() {

	TCCR2B = TCCR2B;				// Write dummy-value to control register
	while (ASSR & _BV(TCN2UB));		// Wait until busy-flag of async is gone

}

static bool led = false;

/*
 * Timer2 interrupt
 */
ISR(TIMER2_OVF_vect) {

	if (led) {
		PORTC |= _BV(PC0);
	} else {
		PORTC &= ~(_BV(PC0));
	}

	led = !led;

}
