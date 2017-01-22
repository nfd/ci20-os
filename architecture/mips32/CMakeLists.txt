add_library(architecture STATIC start.S kernelstack.c exception.c caches.c caches_asm.S kernel_layout.c jump_to_sigma0.S)

# TODO this is a bit weird.
target_link_libraries(architecture soc)

# TODO pass arch from main
target_include_directories(architecture PUBLIC ../../include ../../include/kernel/architecture/mips32)

