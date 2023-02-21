cmake_minimum_required(VERSION 3.10)


project(MyProject)


# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add source files
file(GLOB SOURCES "*.cpp")


add_subdirectory("./googletest")


# Add an executable target
add_executable(main ${SOURCES})

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(main ${SOURCES})
target_link_libraries(main gtest_main)
add_test(NAME example_test COMMAND example)


# # Find and link against GoogleTest
# find_package(GTest REQUIRED)
# target_include_directories(main PRIVATE ${GTEST_INCLUDE_DIRS})
# target_link_libraries(main PRIVATE ${GTEST_LIBRARIES})
