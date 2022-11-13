#include "network_utils.h"

extern "C" {
#include <cyw43.h>
}
#include <lwip/autoip.h>
#include <lwip/apps/mdns.h>
#include <lwip/err.h>
#include <pico/cyw43_arch.h>

namespace network_utils {

err_t setup_mdns(struct netif *netif){
    const auto hostname =  "pico-w-webserver";
    mdns_resp_init();
    return mdns_resp_add_netif(netif, hostname);
}

err_t connect_to_wifi() {
     cyw43_arch_enable_sta_mode();
    // this seems to be the best be can do using the predefined `cyw43_pm_value` macro:
    // cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);
    // however it doesn't use the `CYW43_NO_POWERSAVE_MODE` value, so we do this instead:
    cyw43_wifi_pm(&cyw43_state, cyw43_pm_value(CYW43_NO_POWERSAVE_MODE, 20, 1, 1, 1));
    auto& netif = cyw43_state.netif[CYW43_ITF_STA];
    const auto err = setup_mdns(&netif);
    if (err != ERR_OK) {
        return err;
    }

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return ERR_CONN;
    } else {
        printf("Connected.\n");

        auto ip_addr = netif.ip_addr.addr;
        printf("IP Address: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);
    }
    return ERR_OK;
}

err_t stop_access_point(){
    auto& netif = cyw43_state.netif[CYW43_ITF_AP];
    auto err = autoip_stop(&netif);
    if (err != ERR_OK){
        return err;
    }
    err = mdns_resp_remove_netif(&netif);
    if (err != ERR_OK){
        return err;
    }
    return ERR_OK;
}

err_t setup_access_point() {
    if (!cyw43_is_initialized(&cyw43_state)){
        if (cyw43_arch_init()) {
            printf("failed to initialise\n");
            return ERR_ABRT;
        }
    }

    const char *ap_name = "picow_webserver";
    const char *password = NULL;

    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);
    // sleep mostly to let serial monitor catch the ip log. probably to be removed once mDNS is set up
    sleep_ms(2000);

    auto& netif = cyw43_state.netif[CYW43_ITF_AP];
    const auto err = setup_mdns(&netif);
    if (err != ERR_OK) {
        return err;
    }

    autoip_start(&netif);
    auto link_local_ip_addr = netif_autoip_data(&netif)->llipaddr.addr;
    printf("IP Address: %lu.%lu.%lu.%lu\n",
        link_local_ip_addr & 0xFF,
        (link_local_ip_addr >> 8) & 0xFF,
        (link_local_ip_addr >> 16) & 0xFF,
        link_local_ip_addr >> 24);
    return ERR_OK;
}

} // namespace network_utils