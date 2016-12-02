project(ci20-os C)
cmake_minimum_required(VERSION 3.6)

set(TOOLCHAIN_PREFIX mipsel-unknown-elf-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_LD ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}ranlib)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mips32r2 -EL -O2 -std=c99 -Wall -Werror -Iinclude -G0")

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_LD} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

enable_language(ASM)
set(can_use_assembler TRUE)
set(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS})

#add_subdirectory(libci20)

add_subdirectory(stage1)

# TODO: ideally system/ci20 CMakeLists would include these (since it's a dependency of the system)
add_subdirectory(architecture/mips32)
add_subdirectory(soc/jz4780)
add_subdirectory(driver/jz47xx-uart)
add_subdirectory(driver/jz47xx-timer)

add_subdirectory(system/ci20)
add_subdirectory(kernel)


