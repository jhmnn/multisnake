set(target_name engine)

add_library(${target_name} STATIC
  engine/engine.hpp
  engine/engine.cpp
  engine/input.hpp
  engine/input.cpp
  engine/time.hpp
  engine/time.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/engine/
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    level
    objects
    network
    ncurses
)
