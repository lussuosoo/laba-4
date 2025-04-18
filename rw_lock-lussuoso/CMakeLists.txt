cmake_minimum_required(VERSION 3.10)
project(MyCppProject LANGUAGES CXX)

# Enable C++23 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-agnostic shared library configuration
option(BUILD_SHARED_LIBS "Build shared library" ON)

# Enable testing
enable_testing()

# Configure shared library
add_library(MyLibrary  STATIC#SHARED 
        src/library.cpp
        include/library.hpp
)

# Public headers location
target_include_directories(MyLibrary PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Platform-specific settings
if(MSVC)
    target_compile_options(MyLibrary PRIVATE /W4 /WX)
else()
    target_compile_options(MyLibrary PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(MyLibrary PRIVATE -fPIC)  # Needed for shared libs on Unix
endif()

# Configure tests
add_subdirectory(googletest) #эта строчка вместо следующей, ее закомментила, она была изнасально, этой не было
#find_package(gTest REQUIRED) 



add_executable(MyLibraryTests
        tests/test_basic.cpp
)


#target_include_directories(MyLibraryTests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> ${gtest_SOURCE_DIR}/include)# чтоб библиотека нашлась, я добавила
target_link_libraries(MyLibraryTests
        PRIVATE
        MyLibrary
        GTest::gtest_main # после двоеточия с маленькой буквы
) 

# Enable CTest integration
add_test(NAME MyLibraryTests COMMAND MyLibraryTests)