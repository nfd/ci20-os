add_library(jz47xx-uart STATIC uart.c)

# TODO pass arch from main
target_include_directories(jz47xx-uart PUBLIC ../../include ../../include/architecture/mips32)

target_link_libraries(jz47xx-uart PUBLIC architecture)
