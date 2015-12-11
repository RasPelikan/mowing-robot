#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <util/delay.h>

#include "stateEngine.h"
#include "rtc.h"

/*
 * Find the current state's function and call it
 */
bool do_state() {

	rtc_wakupe();						// see rtc.h
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);// Power Safe - only rtc is active
	sleep_enable();						// Enable sleep
	sleep_mode();						// Enter sleep mode
	sleep_disable();					// First thing to do is disable sleep

	return false;

}

bool do_pwm() {

    uint8_t brightness;

	// increasing brightness
	for (brightness = 0; brightness < 255; ++brightness)
	{
		// set the brightness as duty cycle
		OCR0B = brightness;

		// delay so as to make the user "see" the change in brightness
		_delay_ms(10);
	}

	// decreasing brightness
	for (brightness = 255; brightness > 0; --brightness)
	{
		// set the brightness as duty cycle
		OCR0B = brightness;

		// delay so as to make the user "see" the change in brightness
		_delay_ms(10);
	}

	return false;

}

/*
 * Software reset: Enable Watchdog timer at the shortest interval
 * and go into an infinite loop.
 */
void soft_reset() {

	wdt_enable(WDTO_15MS);
	for (;;) {
	}

}

/*
 * After soft-reset this method has to be call
 */
void disable_wdt_after_softreset() {

	MCUSR = 0;			// re-enable RESET-interrupt on external pin
	wdt_disable();

}

/*
 * Process each state until any state wants to reset the device
 */
void run_stateEngine() {

	bool reset = false;

	do {

		//reset = do_state();
		reset = do_pwm();

	} while (!reset);

	soft_reset();

}
