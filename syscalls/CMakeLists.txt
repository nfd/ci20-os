add_library(syscalls_static STATIC syscalls.S)

target_include_directories(syscalls_static PUBLIC ../include ../include/architecture/mips32)

