cmake_minimum_required(VERSION 3.20)

project(stm32f767zit6_embedded_architecture C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(
    STM32F767xx
    USE_FULL_LL_DRIVER
)

include_directories(
    common/cmsis/Include
    common/cmsis/Device/ST/STM32F7xx/Include
)

function(add_stm32_module TARGET_NAME)
    add_executable(${TARGET_NAME}.elf ${ARGN})

    target_link_options(${TARGET_NAME}.elf PRIVATE
        -T${CMAKE_SOURCE_DIR}/common/linker/STM32F767ZITx_FLASH.ld
        -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET_NAME}.map
    )

    add_custom_command(TARGET ${TARGET_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}.elf> ${CMAKE_BINARY_DIR}/${TARGET_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}.elf> ${CMAKE_BINARY_DIR}/${TARGET_NAME}.bin
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}.elf>
        COMMENT "Generating HEX/BIN and printing memory footprint for ${TARGET_NAME}"
    )
endfunction()

# Decommentare i moduli dopo aver copiato gli header CMSIS e lo startup file in common/:
# add_subdirectory(modules/01_rcc_clock_tree)
# add_subdirectory(modules/02_gpio_exti)
