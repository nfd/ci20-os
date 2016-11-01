# The DDR init is generated from a Python spec.
# This puts the output within the CMake temporary directory which also has object files etc.
set(DDR_C_GENERATED "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/ddr.c")

add_custom_command(OUTPUT ${DDR_C_GENERATED}
	COMMAND python3
	ARGS ddr.py -o ${DDR_C_GENERATED})

add_executable(stage1.elf stage1.S main.c ci20board.c memtest.c ${DDR_C_GENERATED})

target_link_libraries(stage1.elf LINK_PUBLIC ci20mini)
set_target_properties(stage1.elf PROPERTIES
	LINK_DEPENDS stage1/stage1.lds
	LINK_FLAGS "-T stage1.lds")

