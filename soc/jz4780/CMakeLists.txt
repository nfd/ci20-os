add_library(soc-jz4780 STATIC init.c interrupts.c)

target_include_directories(soc-jz4780 PUBLIC ../../include ../../include/architecture/mips32)
