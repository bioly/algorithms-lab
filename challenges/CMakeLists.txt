# Auto-registers every challenge folder named NN_something (01_maximal_square, ...).
# To add a challenge you only create the folder; no edit here is needed.
# CONFIGURE_DEPENDS makes the build re-scan when folders are added.
file(GLOB challenge_dirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/[0-9][0-9]_*)
list(SORT challenge_dirs)

foreach(dir IN LISTS challenge_dirs)
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir}
     AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
    add_subdirectory(${dir})
  endif()
endforeach()
