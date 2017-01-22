add_custom_command(OUTPUT base_vaddr.lds
	COMMAND ./gen_highest_vaddr.sh
	DEPENDS kernel.elf
	)

add_custom_target(gen_highest_vaddr_target DEPENDS base_vaddr.lds)

add_executable(sigma0.elf main.c start.S kernel_was_rebuilt.c)

target_include_directories(sigma0.elf PUBLIC ../include ../include/kernel/architecture/mips32 ../include/kernel/soc/jz4780/include)
target_link_libraries(sigma0.elf PUBLIC syscalls_static)

add_dependencies(sigma0.elf kernel.elf gen_highest_vaddr_target)

set_target_properties(sigma0.elf PROPERTIES
	LINK_DEPENDS sigma0/linker.lds
	LINK_DEPENDS sigma0/gen_highest_vaddr.sh
	LINK_FLAGS "-T linker.lds")

