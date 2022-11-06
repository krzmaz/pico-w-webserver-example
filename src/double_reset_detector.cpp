// Double reset logic based on pico_bootsel_via_double_reset.c from pico sdk

#include <pico/stdlib.h>

namespace double_reset_detector {

bool double_reset_detected = false;

#ifndef PICO_WEBSERVER_DOUBLE_RESET_TIMEOUT_MS
#define PICO_WEBSERVER_DOUBLE_RESET_TIMEOUT_MS 500
#endif

// Doesn't make any sense for a RAM only binary
#if !PICO_NO_FLASH
static const uint32_t magic_token[] = {
        0xf01681de, 0xbd729b29, 0xd359be7a,
};

static uint32_t __uninitialized_ram(magic_location)[count_of(magic_token)];

/* Check for double reset and enter AP mode if detected
 *
 * This function is registered to run automatically before main(). The
 * algorithm is:
 *
 *   1. Check for magic token in memory; enter AP mode if found.
 *   2. Initialise that memory with that magic token.
 *   3. Do nothing for a short while (few hundred ms).
 *   4. Clear the magic token.
 *   5. Continue with normal boot.
 *
 * Resetting the device twice quickly will interrupt step 3, leaving the token
 * in place so that the second boot will go to the bootloader.
 */
static void __attribute__((constructor)) boot_double_tap_check(void) {
    for (uint i = 0; i < count_of(magic_token); i++) {
        if (magic_location[i] != magic_token[i]) {
            // Arm, wait, then disarm and continue booting
            for (i = 0; i < count_of(magic_token); i++) {
                magic_location[i] = magic_token[i];
            }
            busy_wait_us(PICO_WEBSERVER_DOUBLE_RESET_TIMEOUT_MS * 1000);
            magic_location[0] = 0;
            return;
        }
    }
    // Detected a double reset
    magic_location[0] = 0;
    double_reset_detected = true;
}

#endif
} // namespace double_reset_detector