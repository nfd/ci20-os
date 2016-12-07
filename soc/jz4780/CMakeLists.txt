add_library(soc STATIC init.c interrupts.c multicore.c multicore_lowlevel.S)

# TODO this is a bit weird (see circular dependency)
target_link_libraries(soc architecture)

target_include_directories(soc PUBLIC ../../include ../../include/architecture/mips32 ../../include/soc/jz4780/include )
