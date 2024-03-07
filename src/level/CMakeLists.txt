set(target_name level)

add_library(${target_name} STATIC
  level/level.hpp
  level/level.cpp
  level/collision.hpp
  level/collision.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/level/
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    objects
)
