add_library(soc STATIC init.c interrupts.c multicore.c multicore_lowlevel.S)

# TODO this is a bit weird (see circular dependency)
target_link_libraries(soc architecture jz47xx-uart jz47xx-timer)

target_include_directories(soc PUBLIC ../../include ../../include/kernel/architecture/mips32 ../../include/kernel/soc/jz4780/include )
