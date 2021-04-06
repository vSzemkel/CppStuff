
cmake_minimum_required (VERSION 3.13)

project (shm_producer_consumer)

set(SHARED_LIBS "librt.so")

set(PRO_SOURCES "producer.cpp" "common.h")
set(CON_SOURCES "consumer.cpp" "common.h")

add_executable(shm_producer ${PRO_SOURCES})
target_link_libraries(shm_producer ${SHARED_LIBS})

add_executable(shm_consumer ${CON_SOURCES})
target_link_libraries(shm_consumer ${SHARED_LIBS})

