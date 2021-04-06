
cmake_minimum_required (VERSION 3.13)

project (sock_producer_consumer)

set(SHARED_LIBS "librt.so")

set(PRO_SOURCES "producer.cpp" "common.h")
set(CON_SOURCES "consumer.cpp" "common.h")

add_executable(sock_producer ${PRO_SOURCES})
target_link_libraries(sock_producer "libpthread.so" ${SHARED_LIBS})

add_executable(sock_consumer ${CON_SOURCES})
target_link_libraries(sock_consumer ${SHARED_LIBS})

