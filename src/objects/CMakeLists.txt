set(target_name objects)

add_library(${target_name} STATIC
  objects/object.hpp
  objects/object.cpp
  objects/player.hpp
  objects/player.cpp
  objects/spawner.hpp
  objects/spawner.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/objects/
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    engine
    level
)
