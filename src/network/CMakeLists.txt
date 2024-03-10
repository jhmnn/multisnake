set(target_name network)

add_library(${target_name} STATIC
  network/network.hpp
  network/network.cpp
  network/server.hpp
  network/server.cpp
  network/client.hpp
  network/client.cpp

)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/network/
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(
  ${target_name}
  PRIVATE
)
