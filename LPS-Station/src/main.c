#include <avr/io.h>
#include <avr/interrupt.h>

#include "stateEngine.h"
#include "rtc.h"

/*
 * Boot the device (set start-configuration)
 */
void boot() {

	// post-softreset action
	disable_wdt_after_softreset();

	/*
	 * configure MCU
	 */
	DDRC = _BV(PC0);				// PC0 for output

	/*
	 * start timers
	 */
	enable_rtc();
	sei();
	// enable global interrupts

	/*
	 * init PWM on pin PD5 (OC0B)
	 */

	// initialize timer0 in PWM mode
	TCCR0A |= _BV(WGM00) | _BV(WGM01) | _BV(COM0B1);
	TCCR0B |= _BV(CS00);

	// make sure to make OC0B/PD5 pin as output pin
	DDRD = _BV(PD5);

	/*
	 * init ADC
	 */

	// AREF = AVcc
	ADMUX = _BV(REFS0);

	// Enable ADC and set prescaler to 64 = 4MHz / 64 = 64kHz
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);

}

/*
 * Main-routine
 */
int main() {

	// configure MCU and start timers
	boot();

	// process events to fulfill requirements
	run_stateEngine();

	return 0;

}
