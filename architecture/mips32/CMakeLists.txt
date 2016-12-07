add_library(architecture STATIC start.S kernelstack.c)

# TODO this is a bit weird.
target_link_libraries(architecture soc)

# TODO pass arch from main
target_include_directories(architecture PUBLIC ../../include ../../include/architecture/mips32)

