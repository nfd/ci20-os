add_library(system STATIC init.c)

target_include_directories(system PUBLIC ../../include ../../include/kernel/system/ci20/include)

target_link_libraries(system architecture soc)

