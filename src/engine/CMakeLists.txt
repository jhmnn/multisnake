set(target_name engine)

add_library(${target_name} STATIC
  engine/input.hpp
  engine/input.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/engine/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    ncurses
)
