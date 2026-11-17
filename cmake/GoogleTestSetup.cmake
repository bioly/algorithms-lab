# Makes GoogleTest available as GTest::gtest_main.
#
# Strategy: use a GoogleTest installed on the machine (e.g. `brew install googletest`)
# if there is one; otherwise download it at configure time. FIND_PACKAGE_ARGS
# is what asks FetchContent to try find_package() first.
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
  GIT_SHALLOW    TRUE
  FIND_PACKAGE_ARGS NAMES GTest
)

# Windows only: use the same runtime library as our code.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Do not install GoogleTest together with our project.
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)  # provides gtest_discover_tests()
