cmake_minimum_required(VERSION 3.10.2)
project(dc C)

set(CMAKE_C_STANDARD 11)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(dc SHARED dlfcn.c error.c fcntl.c netdb.c stdio.c stdlib.c unistd.c sys/mman.c sys/stat.c sys/socket.c)
target_compile_options(dc PRIVATE -Wpedantic -Wall -Wextra -fPIC)

find_library(LIBDL dl)
if(LIBDL)
  target_link_libraries(dc ${LIBDL})
endif()

find_library(LIBRT rt)
if(LIBRT)
  target_link_libraries(dc ${LIBRT})
endif()

install(TARGETS dc DESTINATION lib)
install(FILES dlfcn.h error.h fcntl.h netdb.h stdio.h stdlib.h unistd.h DESTINATION include/dc)
install(FILES sys/mman.h sys/socket.h sys/stat.h DESTINATION include/dc/sys)

add_executable(server server.c)
target_link_libraries(server dc -lm)
target_compile_options(server PRIVATE -Wpedantic -Wall -Wextra)

add_executable(client client.c)
target_link_libraries(client dc -lm)
target_compile_options(client PRIVATE -Wpedantic -Wall -Wextra)
