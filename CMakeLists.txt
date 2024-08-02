cmake_minimum_required(VERSION 3.27)

project(arch CXX )

SET( CMAKE_CXX_FLAGS "-Ofast -DNDEBUG -std=c++20 -march=native -fpic -ftree-vectorize") 
if (NOT DEFINED ARCH_DIR)
  set(ARCH_DIR "${CMAKE_CURRENT_LIST_DIR}")
endif()

set(ARCH_INCLUDE_DIRS ${ARCH_DIR}/src)

file(GLOB LN_SOURCES  "src/*.cpp" "src/*.hpp" )



# foreach(_cpptarget ln_test)
#   add_executable(${_cpptarget} test/${_cpptarget}.cpp )
#   target_sources(${_cpptarget} PUBLIC test/${_cpptarget}.cpp ${LN_SOURCES})
#   target_link_libraries(${_cpptarget}  PUBLIC Catch2::Catch2WithMain)
#   find_package(Catch2 3 REQUIRED)  
# endforeach()

# foreach(_cubetarget cube)
#   add_executable(${_cubetarget} test/${_cubetarget}.cpp )
#   target_sources(${_cubetarget} PUBLIC test/${_cubetarget}.cpp ${LN_SOURCES})
#  endforeach()

#  foreach(_beadstarget beads)
#   add_executable(${_beadstarget} test/${_beadstarget}.cpp )
#   target_sources(${_beadstarget} PUBLIC test/${_beadstarget}.cpp ${LN_SOURCES})
#   find_package(fmt)
#   target_link_libraries(${_beadstarget} PUBLIC fmt::fmt)
#  endforeach()

#  foreach(_outlinetarget outline)
#  add_executable(${_outlinetarget} test/${_outlinetarget}.cpp )
#  target_sources(${_outlinetarget} PUBLIC test/${_outlinetarget}.cpp ${LN_SOURCES})
# endforeach()

foreach(_scraperstarget skyscrapers)
add_executable(${_scraperstarget} test/${_scraperstarget}.cpp )
target_sources(${_scraperstarget} PUBLIC test/${_scraperstarget}.cpp ${LN_SOURCES})
endforeach()
# foreach(_triangle triangle)
# add_executable(${_triangle} test/${_triangle}.cpp )
# target_sources(${_triangle} PUBLIC test/${_triangle}.cpp ${LN_SOURCES})
# endforeach()

foreach(_mesh mesh)
add_executable(${_mesh} test/${_mesh}.cpp )
target_sources(${_mesh} PUBLIC test/${_mesh}.cpp ${LN_SOURCES})
endforeach()
# foreach(_cube2target cube2)
# add_executable(${_cube2target} test/${_cube2target}.cpp )
# target_sources(${_cube2target} PUBLIC test/${_cube2target}.cpp ${LN_SOURCES})
# find_package(fmt)
# target_link_libraries(${_cube2target} PUBLIC fmt::fmt)
# endforeach()