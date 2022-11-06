#include <string.h>
#include <stdlib.h>

#include "lwip/apps/httpd.h"
#include "lwip/err.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwipopts.h"
#include "ssi.h"
namespace double_reset_detector {
    extern bool double_reset_detected;
}

int connect_to_wifi() {
     cyw43_arch_enable_sta_mode();
    // this seems to be the best be can do using the predefined `cyw43_pm_value` macro:
    // cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);
    // however it doesn't use the `CYW43_NO_POWERSAVE_MODE` value, so we do this instead:
    cyw43_wifi_pm(&cyw43_state, cyw43_pm_value(CYW43_NO_POWERSAVE_MODE, 20, 1, 1, 1));

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return ERR_CONN;
    } else {
        printf("Connected.\n");

        extern cyw43_t cyw43_state;
        auto ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
        printf("IP Address: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);
    }
    return ERR_OK;
}

void run_server() {
    if (connect_to_wifi() != ERR_OK){
        return;
    }
    httpd_init();
    ssi_init();
    printf("Http server initialized.\n");
    // infinite loop for now
    for (;;) {}
}

void setup() {
    if (double_reset_detector::double_reset_detected) {
        printf("Double reset detected!\n");
        // just blink the LED for now
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
    run_server();
}