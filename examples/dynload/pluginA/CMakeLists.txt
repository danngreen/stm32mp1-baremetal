cmake_minimum_required(VERSION 3.22)
set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_LIST_DIR}/../cmake/arm-none-eabi-gcc.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_BUILD_TYPE "RelWithDebInfo")
set(CMAKE_BUILD_TYPE "Release")

include(${CMAKE_SOURCE_DIR}/../cmake/arch_mp15xa7.cmake)

project(
  PluginA
  VERSION 0.1
  DESCRIPTION "PluginA for stm32mp1-baremetal dynload example"
  LANGUAGES C CXX ASM)

add_executable(pluginA pluginA.cc ../../../shared/system/libc_stub.c
                       ../../../shared/system/libcpp_stub.cc)

target_include_directories(pluginA PRIVATE ../)

target_link_libraries(pluginA INTERFACE mp15xa7_arch)
target_link_options(pluginA PRIVATE ${MCU_FLAGS})
target_compile_options(pluginA PRIVATE -ffreestanding -nostdlib -nostartfiles)
target_compile_features(pluginA PRIVATE cxx_std_23)
