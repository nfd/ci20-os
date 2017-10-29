add_executable(kernel.elf main.c memset.c memcpy.c sigma0_interface.c)

target_include_directories(kernel.elf PUBLIC ../include ../include/architecture/mips32)

target_link_libraries(kernel.elf PUBLIC system)

set_target_properties(kernel.elf PROPERTIES
	LINK_DEPENDS ${PROJECT_SOURCE_DIR}/kernel/linker.lds
	LINK_FLAGS "-T ${PROJECT_SOURCE_DIR}/kernel/linker.lds")

# Very evil custom command to trigger sigma0 rebuild after kernel was built.
add_custom_command(TARGET kernel.elf POST_BUILD COMMAND touch ../sigma0/kernel_was_rebuilt.c)
