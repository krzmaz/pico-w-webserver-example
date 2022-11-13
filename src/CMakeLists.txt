set(PROGRAM_NAME pico_w_webserver)

set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata)

if (NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
        file(DOWNLOAD
                https://raw.githubusercontent.com/krzmaz/lwip/e15654409d14a238aec5ed4bd5516063938c9345/src/apps/http/makefsdata/makefsdata
                ${MAKE_FS_DATA_SCRIPT}
                )
endif()
message("Running makefsdata script")
execute_process(COMMAND
        perl ${MAKE_FS_DATA_SCRIPT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
        COMMAND_ERROR_IS_FATAL ANY
        )
file(RENAME fsdata.c my_fsdata.c)


# TODO remove after https://github.com/georgerobotics/cyw43-driver/pull/39 gets into the SDK
# The file has code written for an older version of lwIP hidden behind `#if LWIP_MDNS_RESPONDER` clause
# so we change `LWIP_MDNS_RESPONDER` to a different `NON_EXISTANT_MACRO` to make that `#if` never be true
set(FILE_TO_CHANGE ${PICO_CYW43_DRIVER_PATH}/src/cyw43_lwip.c)
file(READ ${FILE_TO_CHANGE} FILE_CONTENTS)
string(REPLACE "LWIP_MDNS_RESPONDER" "NON_EXISTANT_MACRO" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE ${FILE_TO_CHANGE} "${FILE_CONTENTS}")

add_executable(${PROGRAM_NAME}
        main.cpp
        double_reset_detector.cpp
        network_utils.cpp
        http_server.cpp
        )
target_compile_definitions(${PROGRAM_NAME} PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
target_include_directories(${PROGRAM_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )
target_link_libraries(${PROGRAM_NAME}
        cyw43_driver_base
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_http
        pico_lwip_mdns
        pico_stdlib
        hardware_adc
        )

pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)
suppress_tinyusb_warnings()

pico_add_extra_outputs(${PROGRAM_NAME})
