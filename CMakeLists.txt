project(libkoralle)

cmake_minimum_required(VERSION 2.6.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

# flags
option(BUILD_TESTS "Build the tests" ON)

# search deps
find_package(Qt4 4.7.0 REQUIRED)


# set variables
set(koralle_LIB_ABI_VERSION 0)
set(koralle_LIB_NAME  koralle${koralle_LIB_ABI_VERSION} )

set(koralle_LIB_VERSION_MAJOR 0)
set(koralle_LIB_VERSION_MINOR 1)
set(koralle_LIB_VERSION_PATCH 0)
set(koralle_LIB_VERSION ${koralle_LIB_VERSION_MAJOR}.${koralle_LIB_VERSION_MINOR}.${koralle_LIB_VERSION_PATCH})

set(koralle_LIB_SOVERSION 1) # bump for binary-incompatible changes
set(koralle_LIB_API_VERSION 0) # bump for API extensions in a binary-compatible way
set(koralle_LIB_PATCH_VERSION 0) # bump for no changes to API
set(koralle_LIB_LIBVERSION ${koralle_LIB_SOVERSION}.${koralle_LIB_API_VERSION}.${koralle_LIB_PATCH_VERSION})


# helpers for releasing
set(ARCHIVE_NAME lib${koralle_LIB_NAME}-${koralle_LIB_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(distcheck
    COMMAND cd ${CMAKE_BINARY_DIR}
    && rm -rf ${ARCHIVE_NAME}
    && tar xf ${ARCHIVE_NAME}.tar.bz2
    && mkdir ${ARCHIVE_NAME}/build
    && cd ${ARCHIVE_NAME}/build
    && cmake -DCMAKE_INSTALL_PREFIX=../install ..
    && make
    && make install
    && make check
)
add_dependencies(distcheck dist)


# other files
configure_file(koralle.pc.in ${CMAKE_BINARY_DIR}/${koralle_LIB_NAME}.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/${koralle_LIB_NAME}.pc
    DESTINATION lib${LIB_SUFFIX}/pkgconfig
)

# sources
include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)
add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
)

add_subdirectory(src)
add_subdirectory(includes)

# if (BUILD_TESTS)
#     add_subdirectory(tests)
# endif (BUILD_TESTS)
