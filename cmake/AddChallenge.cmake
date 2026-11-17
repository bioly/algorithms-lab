# add_challenge(<name>)
#
# Call it from a challenge folder's CMakeLists.txt. It expects these files in
# that folder:
#   <name>.h / <name>.cpp     the solution
#   <name>_test.cpp           GoogleTest tests (required)
#   <name>_trace.cpp          step-by-step demo (optional)
#
# It creates these targets:
#   <name>_lib     the solution as a library
#   <name>_test    the test executable
#   <name>_trace   the demo executable (if the file exists and traces are enabled)
#
# Every test is labeled with <name>, so `ctest -L <name>` runs just this challenge.
function(add_challenge name)
  add_library(${name}_lib ${name}.cpp)
  target_include_directories(${name}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${name}_lib PUBLIC algolab_options)

  add_executable(${name}_test ${name}_test.cpp)
  target_link_libraries(${name}_test PRIVATE ${name}_lib GTest::gtest_main)
  # PRE_TEST: discover the individual tests when `ctest` runs, not at build time.
  gtest_discover_tests(${name}_test
                       DISCOVERY_MODE PRE_TEST
                       PROPERTIES LABELS ${name})

  if(ALGOLAB_BUILD_TRACES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name}_trace.cpp)
    add_executable(${name}_trace ${name}_trace.cpp)
    target_link_libraries(${name}_trace PRIVATE algolab_options)
  endif()
endfunction()
