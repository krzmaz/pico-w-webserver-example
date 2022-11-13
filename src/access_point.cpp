#include "access_point.h"

#include <cyw43.h>
#include <lwip/autoip.h>
#include <pico/cyw43_arch.h>

namespace access_point {

int stop_access_point(){
    autoip_stop(&cyw43_state.netif[CYW43_ITF_AP]);
    return 0;
}

int setup_access_point() {
    if (!cyw43_is_initialized(&cyw43_state)){
        if (cyw43_arch_init()) {
            printf("failed to initialise\n");
            return 1;
        }
    }

    const char *ap_name = "picow_webserver";
    const char *password = NULL;

    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);
    // sleep mostly to let serial monitor catch the ip log. probably to be removed once mDNS is set up
    sleep_ms(2000);

    autoip_start(&cyw43_state.netif[CYW43_ITF_AP]);
    auto link_local_ip_addr = netif_autoip_data(&cyw43_state.netif[CYW43_ITF_AP])->llipaddr.addr;
    printf("IP Address: %lu.%lu.%lu.%lu\n",
        link_local_ip_addr & 0xFF,
        (link_local_ip_addr >> 8) & 0xFF,
        (link_local_ip_addr >> 16) & 0xFF,
        link_local_ip_addr >> 24);
    return 0;
}

} // namespace access_point