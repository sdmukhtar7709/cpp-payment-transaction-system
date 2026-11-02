cmake_minimum_required(VERSION 3.16)
project(PayCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(paycore
    src/Transaction.cpp
    src/Storage.cpp
    src/TransactionManager.cpp
)
target_include_directories(paycore PUBLIC include)

if(MSVC)
    target_compile_options(paycore PRIVATE /W4)
else()
    target_compile_options(paycore PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(paycore_app src/main.cpp)
target_link_libraries(paycore_app PRIVATE paycore)

enable_testing()
add_executable(paycore_tests tests/test_transactions.cpp)
target_link_libraries(paycore_tests PRIVATE paycore)
add_test(NAME PayCoreTests COMMAND paycore_tests)
