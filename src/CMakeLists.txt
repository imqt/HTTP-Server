#PROJECT SETTINGS
cmake_minimum_required(VERSION 3.16.3)
project(http_server C)

set(CMAKE_C_STANDARD 11)

#LOCAL LIBRARIES
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(dc SHARED dc_lib/dlfcn.c dc_lib/error.c dc_lib/fcntl.c dc_lib/netdb.c dc_lib/stdio.c dc_lib/stdlib.c dc_lib/unistd.c dc_lib/sys/mman.c dc_lib/sys/stat.c dc_lib/sys/socket.c config/config.c config/config.h)
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
install(FILES dc_lib/dlfcn.h dc_lib/error.h dc_lib/fcntl.h dc_lib/netdb.h dc_lib/stdio.h dc_lib/stdlib.h dc_lib/unistd.h DESTINATION include/dc)
install(FILES dc_lib/sys/mman.h dc_lib/sys/socket.h dc_lib/sys/stat.h DESTINATION include/dc/sys)

#SERVER LIBRARIES
add_library(pool server/pool.h server/pool.c)
target_link_libraries(pool dc pthread http)
target_compile_options(pool PRIVATE -Wpedantic -Wall -Wextra)

add_library(http http/request.h http/request.c http/response.h http/response.c)
target_compile_options(http PRIVATE -Wpedantic -Wall -Wextra)

add_library(config config/config.h config/config.c)
target_compile_options(config PRIVATE -Wpedantic -Wall -Wextra)

#EXECUTABLES
add_executable(server server.c)
target_link_libraries(server dc pool config)
target_compile_options(server PRIVATE -Wpedantic -Wall -Wextra)

add_executable(client client.c)
target_link_libraries(client dc pool http config)
target_compile_options(client PRIVATE -Wpedantic -Wall -Wextra)

add_executable(config-test config-test.c)
target_link_libraries(config-test dc pool http config)
target_compile_options(config-test PRIVATE -Wpedantic -Wall -Wextra)

#NCURSES
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(edit-config edit-config.c edit-config.h ncurses/page.c ncurses/page.h ncurses/line.c ncurses/line.h)
target_link_libraries(edit-config ${CURSES_LIBRARIES} -lncurses)
target_compile_options(edit-config PRIVATE -Wpedantic -Wall -Wextra)