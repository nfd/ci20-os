add_library(system STATIC init.c)

target_include_directories(system PUBLIC ../../include)

target_link_libraries(system INTERFACE soc-jz4780 jz47xx-uart jz47xx-timer)

