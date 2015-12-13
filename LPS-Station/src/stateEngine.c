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

	set_sleep_mode(SLEEP_MODE_IDLE);	// IDLE - we still need PWM
	sleep_enable();						// Enable sleep
	sleep_mode();						// Enter sleep mode
	rtc_wakupe();						// see rtc.h
	sleep_disable();					// First thing to do is disable sleep

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

		reset = do_state();

	} while (!reset);

	soft_reset();

}
