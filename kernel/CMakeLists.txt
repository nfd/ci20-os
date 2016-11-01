add_executable(kernel.elf start.S main.c)

target_link_libraries(kernel.elf LINK_PUBLIC ci20)
set_target_properties(kernel.elf PROPERTIES
	LINK_DEPENDS kernel/linker.lds
	LINK_FLAGS "-T linker.lds")

