
cmake_minimum_required (VERSION 3.13)

project (numf)

option(COMMAND_HISTORY "Build numf with comman history support" OFF)

set(NCUR_SUFFIX "")
set(SHARED_LIBS "libbsd.so" "libm.so" "libpthread.so")

if (COMMAND_HISTORY)
    set(NCUR_SUFFIX "_ncur")
    list(APPEND SHARED_LIBS "libncurses.so")
endif()

set(SOURCES "numf${NCUR_SUFFIX}.c" "numf${NCUR_SUFFIX}.h")

add_executable(numf ${SOURCES})
target_link_libraries(numf ${SHARED_LIBS})
