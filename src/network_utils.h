#pragma once

#include <lwip/err.h>

namespace network_utils {
    err_t connect_to_wifi();

    err_t stop_access_point();

    err_t setup_access_point();
} // namespace network_utils