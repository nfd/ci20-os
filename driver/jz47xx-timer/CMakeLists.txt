add_library(jz47xx-timer STATIC timer.c)

# TODO pass arch from main
target_include_directories(jz47xx-timer PUBLIC ../../include ../../include/architecture/mips32)

target_link_libraries(jz47xx-timer PUBLIC architecture)
