#include <string.h>
#include <stdlib.h>

#include <lwip/err.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include "double_reset_detector.h"
#include "http_server.h"
#include "network_utils.h"

#include "lwipopts.h"

void setup() {
    if (double_reset_detector::double_reset_detected) {
        printf("Double reset detected!\n");
        network_utils::setup_access_point();
        http_server::start_http_server();
        while (true) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            sleep_ms(500);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            sleep_ms(500);
        }
    }
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    // turn on LED to distinguish from BOOTSEL mode
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    setup();
    if (network_utils::connect_to_wifi() != ERR_OK) {
        return 1;
    }
    http_server::start_http_server();
    while (true) {};
}