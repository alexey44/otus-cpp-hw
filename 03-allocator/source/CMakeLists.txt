cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(allocator VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

SET(TARGET_H
#    IpFilter.h
   )

SET(TARGET_SRC
#    IpFilter.cpp
    )

add_executable(03-allocator main.cpp ${TARGET_H} ${TARGET_SRC})

set_target_properties(03-allocator
                      PROPERTIES
                      CXX_STANDARD 11
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(03-allocator PRIVATE "${CMAKE_BINARY_DIR}")
#////////////////////////////////////////////////
target_compile_options(03-allocator PRIVATE
    -Wall -Wextra -pedantic -Werror
)
#////////////////////////////////////////////////

install(TARGETS 03-allocator RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
message(${PROJECT_VERSION_PATCH})
