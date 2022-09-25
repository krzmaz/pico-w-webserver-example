set(PROGRAM_NAME pico_w_webserver)

if (CMAKE_HOST_WIN32)
	set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata.py)
	if(NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
		file(DOWNLOAD 
			https://gist.githubusercontent.com/rspeir/99e6c28e98930872f684bbe2f7aa2dee/raw/8d2a0d904dc4b67872e6f8c2b7cb3d8be5666beb/makefsdata.py
			${MAKE_FS_DATA_SCRIPT}
			)
	endif()
	find_package(Python3 COMPONENTS Interpreter REQUIRED)
	execute_process(COMMAND
			${Python3_EXECUTABLE} ${MAKE_FS_DATA_SCRIPT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
			RESULT_VARIABLE FSDATA_RESULT
			)
else()
	set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata)
	if (NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
		file(DOWNLOAD
			https://raw.githubusercontent.com/krzmaz/lwip/e15654409d14a238aec5ed4bd5516063938c9345/src/apps/http/makefsdata/makefsdata
			${MAKE_FS_DATA_SCRIPT}
			)
	endif()
	execute_process(COMMAND
			perl ${MAKE_FS_DATA_SCRIPT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
			ECHO_OUTPUT_VARIABLE
			ECHO_ERROR_VARIABLE
			COMMAND_ERROR_IS_FATAL ANY
			)
endif()

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
