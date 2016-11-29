add_library(architecture STATIC peekpoke.S)

# TODO pass arch from main
target_include_directories(architecture PUBLIC ../../include ../../include/architecture/mips32)

