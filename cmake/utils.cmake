# see https://github.com/raspberrypi/pico-sdk/issues/1282
function(suppress_pico_sdk_warnings)
        set_source_files_properties(
                ${PICO_SDK_PATH}/src/rp2_common/pico_async_context/async_context_threadsafe_background.c
                PROPERTIES
                COMPILE_FLAGS "-Wno-unused-function")
endfunction()

function(get_pico_sdk_import_cmake)
        set(PICO_SDK_IMPORT_CMAKE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/pico_sdk_import.cmake)

        if (NOT EXISTS ${PICO_SDK_IMPORT_CMAKE})
                file(DOWNLOAD
                        https://raw.githubusercontent.com/raspberrypi/pico-sdk/2e6142b15b8a75c1227dd3edbe839193b2bf9041/external/pico_sdk_import.cmake
                        ${PICO_SDK_IMPORT_CMAKE}
                        )
        endif()
endfunction()
