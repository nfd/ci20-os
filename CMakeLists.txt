project(ci20-os C)
cmake_minimum_required(VERSION 3.6)

set(TOOLCHAIN "clang" CACHE STRING "Toolchain to use (clang or gcc)")
set_property(CACHE TOOLCHAIN PROPERTY STRINGS clang gcc)

if(TOOLCHAIN STREQUAL "clang")
	set(CLANG_PATH CACHE STRING "Directory containing clang binary")
	set(CMAKE_C_COMPILER ${CLANG_PATH}clang)
	set(CMAKE_ASM_COMPILER ${CLANG_PATH}clang)
	set(CMAKE_AR ${CLANG_PATH}llvm-ar)
	set(CMAKE_LD ${CLANG_PATH}clang)
	set(CMAKE_RANLIB ${CLANG_PATH}llvm-ranlib)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mips32r2 -EL -target mipsel-unknown-gnu -nostdlib -nostdinc -fno-builtin")
	set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")
	# Explicitly use lld for linking, rather than attempting to use clang, which things it's talking to gcc.
	set(CMAKE_C_LINK_EXECUTABLE "${CLANG_PATH}ld.lld <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
elseif(TOOLCHAIN STREQUAL "gcc")
	set(TOOLCHAIN_PREFIX mipsel-unknown-elf- CACHE STRING "Prepended to toolchain executables")
	set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
	set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
	set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
	set(CMAKE_LD ${TOOLCHAIN_PREFIX}ld)
	set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}ranlib)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -G0 -mips32r2 -EL") # don't use small data, mips32r2 arch, little endian
	set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mips3")
	set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_LD} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
else()
	message(FATAL_ERROR "Unknown toolchain of ${TOOLCHAIN} specified.")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=c99 -Wall -Werror")


enable_language(ASM)
set(can_use_assembler TRUE)

add_subdirectory(stage1)

# TODO: ideally system/ci20 CMakeLists would include these (since it's a dependency of the system)
add_subdirectory(architecture/mips32)
add_subdirectory(soc/jz4780)
add_subdirectory(driver/jz47xx-uart)
add_subdirectory(driver/jz47xx-timer)

add_subdirectory(system/ci20)
add_subdirectory(kernel)

# Userspace
add_subdirectory(syscalls)
add_subdirectory(sigma0)


# Combined kernel+sigma0 builder
add_custom_command(OUTPUT kernel_combined.elf
	COMMAND ${PROJECT_SOURCE_DIR}/kernel/build_combined_kernel.py ${PROJECT_SOURCE_DIR}/saruman $<TARGET_FILE:kernel.elf> $<TARGET_FILE:sigma0.elf> kernel_combined.elf
	DEPENDS kernel.elf sigma0.elf ${PROJECT_SOURCE_DIR}/kernel/build_combined_kernel.py
	COMMENT "Creating kernel+sigma0 combined ELF")

add_custom_target(kernel_combined ALL DEPENDS kernel_combined.elf)
#set_target_properties(kernel_combined PROPERTIES LINKER_LANGUAGE C)

