project(stalemate)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)

if (STALEMATE_BACKTRACE_LIBUNWIND)
	add_definitions(-DSTALEMATE_BACKTRACE_LIBUNWIND=1)
endif()

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	add_definitions(-Wall)
	add_definitions(-std=c++11)
endif()

find_package(Threads)

add_executable(stalemate_test
	${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)

target_link_libraries(stalemate_test
	${CMAKE_THREAD_LIBS_INIT} /usr/lib/x86_64-linux-gnu/libunwind.a /usr/lib/x86_64-linux-gnu/libunwind-x86_64.a lzma)
