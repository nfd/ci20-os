add_executable(kernel.elf start.S main.c)

target_include_directories(kernel.elf PUBLIC ../include ../include/architecture/mips32)

target_link_libraries(kernel.elf PUBLIC system)

set_target_properties(kernel.elf PROPERTIES
	LINK_DEPENDS kernel/linker.lds
	LINK_FLAGS "-T linker.lds")

