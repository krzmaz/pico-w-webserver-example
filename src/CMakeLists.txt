set(PROGRAM_NAME pico_w_webserver)

set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata)

if (NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
        file(DOWNLOAD
                https://raw.githubusercontent.com/lwip-tcpip/lwip/239918ccc173cb2c2a62f41a40fd893f57faf1d6/src/apps/http/makefsdata/makefsdata
                ${MAKE_FS_DATA_SCRIPT}
                )
endif()
execute_process(COMMAND
        perl ${MAKE_FS_DATA_SCRIPT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )
execute_process(COMMAND
        mv fsdata.c fsdata_custom.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )

add_executable(${PROGRAM_NAME}
        main.cpp
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
        pico_stdlib
        )

pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)
suppress_tinyusb_warnings()

pico_add_extra_outputs(${PROGRAM_NAME})
