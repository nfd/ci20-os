add_library(ci20 STATIC uart.c peekpoke.S pllclock.c timer.c init.c interrupts.c)
add_library(ci20mini STATIC uart.c peekpoke.S pllclock.c timer_basic.c init_basic.c)

target_include_directories(ci20 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ../include)
target_include_directories(ci20mini PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ../include)

