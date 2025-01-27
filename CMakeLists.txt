# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Set the right board
set(PICO_BOARD pico_w)

# Include build functions from Pico SDK
include(pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(pico_midi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    access_point.c
    settings.c
    utils.c
    state.c
    dhcpserver/dhcpserver.c
    dnsserver/dnsserver.c
    status_dispatcher.c
    usb_descriptors.c
    main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
    ${CMAKE_CURRENT_LIST_DIR}/dnsserver
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE    
    pico_stdlib
    pico_cyw43_arch_lwip_poll
    tinyusb_device
    tinyusb_board
    pico_multicore
    pico_unique_id
    hardware_i2c
)