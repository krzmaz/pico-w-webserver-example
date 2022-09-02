set(PROGRAM_NAME pico_w_webserver)

set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/makefsdata.py)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

execute_process(COMMAND
		${Python3_EXECUTABLE} ${MAKE_FS_DATA_SCRIPT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		RESULT_VARIABLE FSDATA_RESULT
		)

file(RENAME fsdata.c my_fsdata.c)

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
        hardware_adc
        )

pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)
suppress_tinyusb_warnings()

pico_add_extra_outputs(${PROGRAM_NAME})
