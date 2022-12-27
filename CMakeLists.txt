cmake_minimum_required(VERSION 3.20)

set(CSTANDARD "-std=gnu99")
set(CWARN     "-Wall -Wextra -Wstrict-prototypes -Wshadow -Wdouble-promotion -Wundef -fno-common -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable")


# Cmake test use nosys.spec for testing arm-none-eabi-gcc
set(CMAKE_EXE_LINKER_FLAGS_INIT "-specs=nosys.specs -specs=nano.specs")

set(PREFIX arm-none-eabi-)
set(CTUNING   "-fno-builtin -ffunction-sections -fdata-sections")
set(CMCU      "-mcpu=cortex-m3 -DSTM32F103xB -mthumb -DUSE_HAL_DRIVER")

if( BUILD STREQUAL "release")

    set(CDEBUG    "-fanalyzer -DLOG_LEVEL=0")
    set(COPT      "-Os")

elseif( BUILD STREQUAL "debug")

    set(CDEBUG    "-Og -g3 -fanalyzer -DLOG_LEVEL=${LOG_LEVEL}")
    set(COPT      "")

endif()



set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING}")

# program names
set(CPP   ${PREFIX}g++)
set(CC     ${PREFIX}gcc)
set(STRIP ${PREFIX}strip)
set(OBJCOPY  ${PREFIX}objcopy)
set(OBJDUMP  ${PREFIX}objdump)
set(SIZE  ${PREFIX}size)



# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${CPP})
set(CMAKE_C_COMPILER   ${CC})
set(CMAKE_ASM_COMPILER ${CC})

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

project(Embedded C ASM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# Project setup
set(INC_FILES src/app/main.c
              src/mcu/stm32f103c8/clock.c
              src/mcu/stm32f103c8/usart.c

              src/shared/fifo/fifo.c
              src/shared/esp/esp.c
              src/shared/string/string.c
              src/shared/retarget/retarget.c

              src/mcu/stm32f103c8/stm32f1xx_hal_msp.c
              src/mcu/stm32f103c8/stm32f1xx_it.c

              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_tim.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_rcc.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_gpio.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_dma.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_cortex.c

              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_pwr.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_flash.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_exti.c
              src/mcu/stm32f103c8/HAL_Driver/Src/stm32f1xx_hal_uart.c

              src/mcu/stm32f103c8/system_stm32f1xx.c
              src/mcu/stm32f103c8/startup_stm32f103xb.s
)

add_executable(${PROJECT_NAME} ${INC_FILES})


set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")


target_link_options(${PROJECT_NAME} PRIVATE
    -T../src/mcu/stm32f103c8/STM32F103C8Tx_FLASH.ld
    -mcpu=cortex-m3
    -mthumb
    -lc
    -lm
    -lc_nano
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -Wl,--relax
)

add_custom_target(binary   ALL     ${OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin && ${SIZE} --format=berkeley ${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})

add_custom_target(flash JFlash -openprj../src/mcu/stm32f103c8/device/jlink.jflash -open${PROJECT_NAME}.bin,0x8000000 -auto -startapp -exit DEPENDS binary)


target_include_directories(${PROJECT_NAME} PUBLIC "src/mcu/stm32f103c8")
target_include_directories(${PROJECT_NAME} PUBLIC "src/mcu/stm32f103c8/HAL_Driver/Inc")
target_include_directories(${PROJECT_NAME} PUBLIC "src/mcu/stm32f103c8/HAL_Driver/Inc/Legacy")
target_include_directories(${PROJECT_NAME} PUBLIC "src/mcu/stm32f103c8/CMSIS/Device/ST/STM32F1xx/Include")
target_include_directories(${PROJECT_NAME} PUBLIC "src/mcu/stm32f103c8/CMSIS/Include")
target_include_directories(${PROJECT_NAME} PUBLIC "src/peripherals")
target_include_directories(${PROJECT_NAME} PUBLIC "src/shared")

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")