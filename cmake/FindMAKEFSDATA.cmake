# Finds (or builds) the makefsdata executable
#
# This will define the following variables
#
#    MAKEFSDATA_FOUND
#
# and the following imported targets
#
#     MAKEFSDATA
#

if (NOT MAKEFSDATA_FOUND)

    include(ExternalProject)
    find_program(MAKE_EXECUTABLE NAMES gmake make mingw32-make REQUIRED)
    # We need to use the main CMakeLists.txt from lwip, but we'll build only makefsdata
    # set(MAKEFSDATA_SOURCE_DIR ${PICO_SDK_PATH}/lib/lwip)
    set(MAKEFSDATA_BINARY_DIR ${CMAKE_BINARY_DIR}/makefsdata)

    set(MAKEFSDATA_BUILD_TARGET MAKEFSDATABuild)
    set(MAKEFSDATA_TARGET makefsdata)

    # horrible, horrible thing because makefsdata.c has a #ifdef __linux__ clause not caring about macs
    if (APPLE)
        message(WARNING "Setting up the linux define for apple, this might break things")
        set(EXTRA_ARGS "-DCMAKE_C_FLAGS:STRING=\"-D__linux__\"")
    endif()

    if (NOT TARGET ${MAKEFSDATA_BUILD_TARGET})
        ExternalProject_Add(${MAKEFSDATA_BUILD_TARGET}
                PREFIX makefsdata
                GIT_REPOSITORY https://github.com/krzmaz/lwip.git
                GIT_TAG origin/makefsdata-cmake-fix
                GIT_SHALLOW true
                BINARY_DIR ${MAKEFSDATA_BINARY_DIR}
                BUILD_ALWAYS 1 # force dependency checking
                CMAKE_ARGS ${EXTRA_ARGS}
                BUILD_COMMAND ${CMAKE_COMMAND} --build ${MAKEFSDATA_BINARY_DIR} -t makefsdata
                INSTALL_COMMAND ""
                )
    endif()

    # depending on the platform the executable will be in different path
    if(WIN32)
        set(MAKEFSDATA_EXECUTABLE ${MAKEFSDATA_BINARY_DIR}/contrib/ports/win32/example_app/makefsdata)
    else()
        set(MAKEFSDATA_EXECUTABLE ${MAKEFSDATA_BINARY_DIR}/contrib/ports/unix/example_app/makefsdata)
    endif()

    if(NOT TARGET ${MAKEFSDATA_TARGET})
        add_executable(${MAKEFSDATA_TARGET} IMPORTED)
    endif()
    set_property(TARGET ${MAKEFSDATA_TARGET} PROPERTY IMPORTED_LOCATION
            ${MAKEFSDATA_EXECUTABLE})

    add_dependencies(${MAKEFSDATA_TARGET} ${MAKEFSDATA_BUILD_TARGET})
    set(MAKEFSDATA_FOUND 1)
endif()
