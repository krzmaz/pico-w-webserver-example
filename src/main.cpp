#include <string.h>
#include <stdlib.h>

#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwipopts.h"
#include "cgi.h"
#include "ssi.h"

void run_server() {
    httpd_init();
    ssi_init();
    cgi_init();
    printf("Http server initialized.\n");
    // infinite loop for now
    for (;;) {}
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();
    // this seems to be the best be can do using the predefined `cyw43_pm_value` macro:
    // cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);
    // however it doesn't use the `CYW43_NO_POWERSAVE_MODE` value, so we do this instead:
    cyw43_wifi_pm(&cyw43_state, cyw43_pm_value(CYW43_NO_POWERSAVE_MODE, 20, 1, 1, 1));

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");

        extern cyw43_t cyw43_state;
        auto ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
        printf("IP Address: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);
    }
    // turn on LED to signal connected
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);


    run_server();
}
