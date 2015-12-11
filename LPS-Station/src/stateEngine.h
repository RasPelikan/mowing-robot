#include <avr/io.h>

/*
 * Process each state until any state wants to reset the device
 */
void run_stateEngine();

/*
 * Watchdog-timer is use to force a software-reset.
 * After soft-reset this method has to be called in the boot sequence.
 */
void disable_wdt_after_softreset();
